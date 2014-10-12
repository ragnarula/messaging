#ifndef MESSAGING_HPP
#define MESSAGING_HPP

#include <vector>
#include "globals.h"

namespace messaging{

template<typename T>
class IMessageListener;

template <typename T>
class IMessageContext {
    friend class IMessageListener<T>;

    template <typename U>
    friend void send(IMessageContext<U>* ptrContext, const U& _msg);

    template <typename U>
    friend void attach(IMessageContext<U>* ptrContext, IMessageListener<U>* ptrListener);

    template <typename U>
    friend void detach(IMessageContext<U>* ptrContext, IMessageListener<U>* ptrListener);

    std::vector<IMessageListener<T>* > listeners;

    void attachListener( IMessageListener<T>* _listener);
    void detachListener( IMessageListener<T>* _listener);
    void dispatch(const T& _msg);
public:
    IMessageContext();
    //copy
    IMessageContext(const IMessageContext&);
    //move
    //operator=
    IMessageContext& operator=(const IMessageContext&);
    ~IMessageContext();
};

template <typename T>
IMessageContext<T>::IMessageContext(){}

template <typename T>
IMessageContext<T>::IMessageContext(const IMessageContext<T> &other) :
    listeners(other.listeners)
{
    LOG(__PRETTY_FUNCTION__);
    typename std::vector<IMessageListener<T>* >::iterator i;
    for(i = listeners.begin(); i != listeners.end(); ++i){
        (*i)->messaging::template IMessageListener<T>::attachContext(this);
    }
}

template <typename T>
IMessageContext<T>& IMessageContext<T>::operator=(const IMessageContext &other)
{
    LOG(__PRETTY_FUNCTION__);
    listeners = other.listeners;
    typename std::vector<IMessageListener<T>* >::iterator i;
    for(i = listeners.begin(); i != listeners.end(); ++i){
        (*i)->messaging::template IMessageListener<T>::attachContext(this);
    }
    return *this;
}

template <typename T>
void IMessageContext<T>::attachListener( IMessageListener<T>* _listener)
{
    LOG(__PRETTY_FUNCTION__);
    listeners.push_back(_listener);
}

template <typename T>
void IMessageContext<T>::detachListener( IMessageListener<T>* _listener)
{
    LOG(__PRETTY_FUNCTION__);
    listeners.erase(std::remove(listeners.begin(), listeners.end(), _listener), listeners.end());
}

template <typename T>
void IMessageContext<T>::dispatch(const T& _msg)
{
    LOG(__PRETTY_FUNCTION__);
    typename std::vector<IMessageListener<T>* >::iterator i;
    for(i = listeners.begin(); i != listeners.end(); ++i){
        (*i)->handleMessage(_msg);
    }
}

template <typename T>
IMessageContext<T>::~IMessageContext()
{
    //detach from all listeners so they dont try to detach from this context when they are destructed
    LOG(__PRETTY_FUNCTION__);
    typename std::vector<IMessageListener<T>* >::iterator i;
    for(i = listeners.begin(); i != listeners.end(); ++i){
        (*i)->messaging::template IMessageListener<T>::detachContext(this);
    }
}

//IMEssageListener Class

template<typename T>
class IMessageListener {
    friend class IMessageContext<T>;

    template <typename U>
    friend void attach(IMessageContext<U>* ptrContext, IMessageListener<U>* ptrListener);

    template <typename U>
    friend void detach(IMessageContext<U>* ptrContext, IMessageListener<U>* ptrListener);

    std::vector<IMessageContext<T>* > contexts;

    void attachContext( IMessageContext<T>* _listener);
    void detachContext( IMessageContext<T>* _listener);
    virtual void handleMessage(const T&)  = 0;
public:
    IMessageListener();
    //copy
    IMessageListener(const IMessageListener&);
    //move
    //operator=
    IMessageListener& operator=(const IMessageListener&);
    virtual ~IMessageListener();
};

template <typename T>
IMessageListener<T>::IMessageListener(){}

template <typename T>
IMessageListener<T>::IMessageListener(const IMessageListener<T> &other) :
    contexts(other.contexts)
{
    typename std::vector<IMessageContext<T>* >::iterator i;
    for(i = contexts.begin(); i != contexts.end(); ++i){
        (*i)->messaging::template IMessageContext<T>::attachListener(this);
    }
}

template <typename T>
IMessageListener<T>& IMessageListener<T>::operator=(const IMessageListener<T> &other)
{
    contexts = other.contexts;
    typename std::vector<IMessageContext<T>* >::iterator i;
    for(i = contexts.begin(); i != contexts.end(); ++i){
        (*i)->messaging::template IMessageContext<T>::attachListener(this);
    }
    return *this;
}


template <typename T>
void IMessageListener<T>::attachContext( IMessageContext<T>* _context)
{
    LOG(__PRETTY_FUNCTION__);
    contexts.push_back(_context);
}

template <typename T>
void IMessageListener<T>::detachContext( IMessageContext<T>* _context)
{
    LOG(__PRETTY_FUNCTION__);
    contexts.erase(std::remove(contexts.begin(), contexts.end(), _context), contexts.end());
}

template <typename T>
IMessageListener<T>::~IMessageListener()
{
    //detach from all contexts so they dont try to message missing listener
    LOG(__PRETTY_FUNCTION__);
    typename std::vector<IMessageContext<T>* >::iterator i;
    for(i = contexts.begin(); i != contexts.end(); ++i){
        (*i)->messaging::template IMessageContext<T>::detachListener(this);
    }
}

// The real API

template <typename T>
void send(IMessageContext<T>* ptrContext, const T& _msg)
{
    LOG(__PRETTY_FUNCTION__);
    ptrContext->template IMessageContext<T>::dispatch(_msg);
}

template <typename T>
void attach(IMessageContext<T>* ptrContext, IMessageListener<T>* ptrListener)
{
    LOG(__PRETTY_FUNCTION__);
    ptrContext->template IMessageContext<T>::attachListener(ptrListener);
    ptrListener->template IMessageListener<T>::attachContext(ptrContext);
}

template <typename T>
void detach(IMessageContext<T>* ptrContext, IMessageListener<T>* ptrListener)
{
    LOG(__PRETTY_FUNCTION__);
    ptrContext->template IMessageContext<T>::detachListener(ptrListener);
    ptrListener->template IMessageListener<T>::detachContext(ptrContext);
}

} //namespace messaging

#endif // MESSAGING_HPP
