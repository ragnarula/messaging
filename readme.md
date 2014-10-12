# Header Only C++ Messaging Library
This simple header only library allows easy implemtation of the observer pattern in C++

## Features
+ Lightweight
+ Messages passed between classes can be of any type
+ Allows low coupling between components
+ Copy constructor and operator= implemented to avoid dangling pointer issues

## Useage
+ Message senders can inherit from the IMessageContext<MESSAGE_TYPE> interface class
+ Message Listeners should inherit from the IMessageListener<MESSAGE_TYPE> interface class
+ To attach a listener to a context:
	messaging::attach<MESSAGE_TYPE>(&source, &destination);
+ To detach a listener from a context:
	messaging::detach<MESSAGE_TYPE>(&source, &destination);
+ To send a message from context to listener:
	messaging::send<MESSAGE_TYPE>(&source, &destination);
+ For all three functions, the MESSAGE_TYPE must be the same as the type used in the type specifier in the base classes
+ Classes may inherit from multiple context/listener interface classes of different types
+ See the included main.cpp for an example
