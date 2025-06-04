#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "client/requests.h"
#include "common/responses.h"
#include "common/updates/game_update.h"

#include "commands.h"


#define MESSAGE_TYPE_MAP(F)                            \
    F(bool, BOOL)                                      \
    F(ConnectionRequest, CONN_REQ)                     \
    F(ListGamesResponse, LIST_GAMES_RESP)              \
    F(CreateGameCommand, CREATE_GAME_CMD) /*Commands*/ \
    F(JoinGameCommand, JOIN_GAME_CMD)                  \
    F(ListGamesCommand, LIST_GAMES_CMD)                \
    F(SelectTeamCommand, SELECT_TEAM_CMD)              \
    F(StartGameCommand, START_GAME_CMD)                \
    F(BuyGunCommand, BUY_GUN_CMD)                      \
    F(BuyAmmoCommand, BUY_AMMO_CMD)                    \
    F(MoveCommand, MOVE_CMD)                           \
    F(StopPlayerCommand, STOP_PLAYER_CMD)              \
    F(AimCommand, AIM_CMD)                             \
    F(ShootCommand, SHOOT_CMD)                         \
    F(ReloadCommand, RELOAD_CMD)                       \
    F(SwitchWeaponCommand, SWITCH_WEAPON_CMD)          \
    F(PlantBombCommand, PLANT_BOMB_CMD)                \
    F(DefuseBombCommand, DEFUSE_BOMB_CMD)              \
    F(PickUpItemCommand, PICK_UP_ITEM_CMD)             \
    F(LeaveGameCommand, LEAVE_GAME_CMD)                \
    F(GameUpdate, GAME_UPDATE)
#define MESSAGE_TYPE_MAP_LAST(F) F(std::nullptr_t, NONE)


#define GET_MESSAGE_TYPE(type, enumName) type,
#define GET_MESSAGE_TYPE_NC(type, enumName) type
/**
 * @brief Variant type holding the payload for a Message.
 */
typedef std::variant<MESSAGE_TYPE_MAP(GET_MESSAGE_TYPE) MESSAGE_TYPE_MAP_LAST(GET_MESSAGE_TYPE_NC)>
        Sendable;
#undef GET_MESSAGE_TYPE
#undef GET_MESSAGE_TYPE_NC

#define GET_ENUM_NAME(type, enumName) enumName,
#define GET_ENUM_NAME_NC(type, enumName) enumName
/**
 * @enum MessageType
 * @brief Types of messages exchanged over the protocol.
 */
enum class MessageType : uint8_t {
    MESSAGE_TYPE_MAP(GET_ENUM_NAME) MESSAGE_TYPE_MAP_LAST(GET_ENUM_NAME_NC)
};
#undef GET_ENUM_NAME
#undef GET_ENUM_NAME_NC

// Trait forward declaration
template <typename T>
struct MessageTypeTrait;

// Detection trait for supported message types
#define DEF_MESSAGE_TRAIT(type, enumName)                           \
    template <>                                                     \
    struct MessageTypeTrait<type> {                                 \
        static constexpr MessageType value = MessageType::enumName; \
    };
MESSAGE_TYPE_MAP(DEF_MESSAGE_TRAIT)
#undef DEF_MESSAGE_TRAIT

#undef MESSAGE_TYPE_MAP

// @return std::false_type always
// @note Checks if the type T has a corresponding MessageTypeTrait specialization.
template <typename T, typename = void>
struct is_message_supported: std::false_type {};

// @return std::true_type if T is a supported message type
// @note Checks if the type T has a corresponding MessageTypeTrait specialization.
template <typename T>
struct is_message_supported<T, std::void_t<decltype(MessageTypeTrait<T>::value)>>:
        std::true_type {};

/**
 * @class Message
 * @brief Encapsulates a typed message with variant content.
 */
class Message {
    MessageType type;
    Sendable content;

public:
    // Simplified templated constructor using type traits
    template <typename T, typename = std::enable_if_t<is_message_supported<std::decay_t<T>>::value>>
    explicit Message(T&& value):
            type(MessageTypeTrait<std::decay_t<T>>::value), content(std::forward<T>(value)) {}

    // Default to NONE
    Message() noexcept: type(MessageType::NONE), content(nullptr) {}

    ~Message() = default;

    MessageType get_type() const { return type; }

    /**
     * @brief Retrieves the stored content as type T.
     * @tparam T Expected content type.
     * @return Content cast to type T.
     * @throws std::bad_variant_access if stored content is not T.
     */
    template <typename T>
    T get_content() const {
        if (std::holds_alternative<T>(content)) {
            return std::get<T>(content);
        } else {
            throw std::bad_variant_access();
        }
    }
};
