# Protocol Communication Class Diagram

This diagram shows the core protocol communication infrastructure, focusing on the protocol classes, communication threads, and supporting infrastructure. For detailed message types and command/response classes, see [message_classes.md](message_classes.md).

```mermaid
classDiagram
    %% Abstract Base Classes
    class BaseSocket {
        <<interface>>
        +sendsome(data, sz) int
        +recvsome(data, sz) int
        +sendall(data, sz) int
        +recvall(data, sz) int
        +is_stream_send_closed() bool
        +is_stream_recv_closed() bool
        +shutdown(how) void
        +close() int
    }

    class BaseProtocol {
        <<abstract>>
        #socket: shared_ptr~BaseSocket~
        +BaseProtocol(skt)
        +is_closed() bool
        +is_open() bool
        +send(message) void
        +recv() Message
        +close() void
        #serialize_message(message)* payload_t
        #deserialize_message(type, payload)* Message
        +serialize~T~(value) payload_t
        +deserialize~T~(payload) T
        -serialize(type) payload_t
        -deserialize_message_type() MessageType
        -deserialize_message_length() uint16_t
        -pop(payload, size) payload_t
    }

    class Thread {
        <<abstract>>
        +run()* void
        +stop() void
    }

    %% Protocol Implementations
    class ClientProtocol {
        +ClientProtocol(skt)
        +serialize_message(message) payload_t
        +serialize_msg~T~(value) payload_t
        +deserialize_message(type, payload) Message
        +deserialize_msg~T~(payload) T
    }

    class ServerProtocol {
        +ServerProtocol(skt)
        -serialize_message(message) payload_t
        -serialize_msg~T~(value) payload_t
        -deserialize_message(type, payload) Message
        -deserialize_msg~T~(payload) T
    }

    %% Message System (simplified reference)
    class Message {
        -type: MessageType
        -content: Sendable
        +Message(value)
        +Message()
        +get_type() MessageType
        +get_content~T~() T
    }

    %% Communication Threads
    class ClientSender {
        -queue: Queue~Message~&
        -protocol: shared_ptr~ClientProtocol~
        +ClientSender(protocol, queue)
        +run() void
        +stop() void
    }

    class ClientReceiver {
        -queue: Queue~Message~&
        -protocol: shared_ptr~ClientProtocol~
        +ClientReceiver(protocol, queue)
        +run() void
        +stop() void
    }

    class Sender {
        -protocol: ServerProtocol&
        -queue: shared_ptr~Queue~Message~~
        +Sender(protocol, queue)
        +run() void
    }

    class Receiver {
        -player_name: string
        -protocol: ServerProtocol&
        -queue: shared_ptr~Queue~PlayerMessage~~
        +Receiver(player_name, protocol, queue)
        +run() void
    }

    %% Queue
    class Queue~T~ {
        -q: queue~T~
        -max_size: unsigned int
        -closed: bool
        -mtx: mutex
        -is_not_full: condition_variable
        -is_not_empty: condition_variable
        +Queue()
        +Queue(max_size)
        +push(val) void
        +pop() T
        +try_push(val) bool
        +try_pop() T
        +close() void
        +is_closed() bool
    }

    %% Data Structures (supporting protocol)
    class PlayerMessage {
        +player_name: string
        +message: Message
        +PlayerMessage(name, msg)
    }

    %% Relationships
    BaseProtocol <|-- ClientProtocol
    BaseProtocol <|-- ServerProtocol
    BaseProtocol --> BaseSocket : uses
    BaseProtocol --> Message : sends/receives

    Thread <|-- ClientSender
    Thread <|-- ClientReceiver
    Thread <|-- Sender
    Thread <|-- Receiver

    ClientSender --> ClientProtocol : uses
    ClientSender --> Queue : reads from
    ClientReceiver --> ClientProtocol : uses
    ClientReceiver --> Queue : writes to

    Sender --> ServerProtocol : uses
    Sender --> Queue : reads from
    Receiver --> ServerProtocol : uses
    Receiver --> Queue : writes to

    Receiver --> PlayerMessage : creates
```
