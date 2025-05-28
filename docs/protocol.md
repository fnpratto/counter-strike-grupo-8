# Protocol Specification

## Overview

This document outlines the protocol for the project, detailing the structure of messages exchanged between the client
and server. The protocol is designed to facilitate communication in a clear and efficient manner.
It is implemented as a binary protocol, which allows for compact representation of data.

## Message Structure

Each message consists of a header and a body. The header contains the message type and length, while the body contains the actual data.

### Header

The header is fixed-length, while the body can vary in length depending on the message type.

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| message_type | uint8  | 1 | Type of the message |
| length | uint32 | 4 | Length of the body in bytes |

#### Message Types

The message types are defined as the uint8_t value of the [MessageType](./../common/message.h) enum.

### Body

The body structure depends on the message type. Each message type has its own specific structure, which is detailed below.

#### Message Type: [Connection Request](./../client/requests.h#L7)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| ip | string | variable | IP address of the client |
| port | string | variable | Port number of the client |

#### Attribute Type: string

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| length | uint32 | 4 | Length of the string |
| value | char[] | variable | The string value |

#### Message/Attribute Type: bool

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| value | bool | 1 | Boolean value (0 for false, 1 for true) |

#### Message Type: [CreateGameCommand](./../common/commands.h#L23)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| name | string | variable | Name of the game |

#### Message Type: [JoinGameCommand](./../common/commands.h#L35)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| name | string | variable | Name of the game |

#### Message Type: [ListGamesCommand](./../common/commands.h#L47)

It only contains the header.

#### Message Type: [SelectTeamCommand](./../common/commands.h#L53)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| team | enum | 1 | Name of the team |

#### Message Type: [StartGameCommand](./../common/commands.h#L65)

It only contains the header.

#### Message Type: [BuyWeaponCommand](./../common/commands.h#L71)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| weapon | enum | 1 | Type of weapon to purchase |

#### Message Type: [MoveCommand](./../common/commands.h#L83)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| direction | enum | 1 | Direction of movement |
| start | bool | 1 | Whether to start (true) or stop (false) movement |

#### Message Type: [AimCommand](./../common/commands.h#L97)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| x | float | 4 | X-coordinate of aim target |
| y | float | 4 | Y-coordinate of aim target |

#### Message Type: [ShootCommand](./../common/commands.h#L111)

It only contains the header.
The shot is fired towards the player's current aim direction.

#### Message Type: [SwitchWeaponCommand](./../common/commands.h#L125)

| Field Name | Type | Length (bytes) | Description |
|-|-|-|-|
| slot | enum | 1 | Weapon slot to switch to |

#### Message Type: [PlantBombCommand](./../common/commands.h#L137)

It only contains the header.

#### Message Type: [DefuseBombCommand](./../common/commands.h#L143)

It only contains the header.

#### Message Type: [PickUpItemCommand](./../common/commands.h#L149)

It only contains the header.

#### Message Type: [LeaveGameCommand](./../common/commands.h#L155)

It only contains the header.

#### Message Type: [ReloadCommand](./../common/commands.h#L161)

It only contains the header.
