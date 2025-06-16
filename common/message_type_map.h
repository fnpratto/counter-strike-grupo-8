#pragma once

#include "client/requests.h"
#include "common/responses.h"
#include "common/updates/game_update.h"

#include "commands.h"

#define RESPONSES_MAP(F)                                                  \
    F(ListGamesResponse, LIST_GAMES_RESP)                                 \
    F(ShopPricesResponse, SHOP_PRICES_RESP)                               \
    F(HitResponse, HIT_RESP)                                              \
    F(CharactersResponse, CHARACTERS_RESP)                                \
    F(ScoreboardResponse, SCOREBOARD_RESP)                                \
    F(TriedToJoinFullTeamErrorResponse, TRIED_TO_JOIN_FULL_TEAM_ERR_RESP) \
    F(CannotBuyErrorResponse, CANNOT_BUY_ERR_RESP)                        \
    F(RoundEndResponse, ROUND_END_RESP)

#define LOBBY_COMMANDS_MAP(F)             \
    F(CreateGameCommand, CREATE_GAME_CMD) \
    F(JoinGameCommand, JOIN_GAME_CMD)     \
    F(ListGamesCommand, LIST_GAMES_CMD)

#define GAME_COMMANDS_MAP(F)                        \
    F(SelectTeamCommand, SELECT_TEAM_CMD)           \
    F(GetCharactersCommand, GET_CHARACTERS_CMD)     \
    F(SelectCharacterCommand, SELECT_CHARACTER_CMD) \
    F(SetReadyCommand, START_GAME_CMD)              \
    F(GetShopPricesCommand, GET_SHOP_PRICES_CMD)    \
    F(BuyGunCommand, BUY_GUN_CMD)                   \
    F(BuyAmmoCommand, BUY_AMMO_CMD)                 \
    F(MoveCommand, MOVE_CMD)                        \
    F(StopPlayerCommand, STOP_PLAYER_CMD)           \
    F(AimCommand, AIM_CMD)                          \
    F(AttackCommand, ATTACK_CMD)                    \
    F(SwitchItemCommand, SWITCH_ITEM_CMD)           \
    F(ReloadCommand, RELOAD_CMD)                    \
    F(GetScoreboardCommand, GET_SCOREBOARD_CMD)     \
    F(PlantBombCommand, PLANT_BOMB_CMD)             \
    F(DefuseBombCommand, DEFUSE_BOMB_CMD)           \
    F(PickUpItemCommand, PICK_UP_ITEM_CMD)          \
    F(LeaveGameCommand, LEAVE_GAME_CMD)

#define UPDATES_MAP(F) F(GameUpdate, GAME_UPDATE)

#define MESSAGE_TYPE_MAP(F)        \
    F(bool, BOOL)                  \
    F(ConnectionRequest, CONN_REQ) \
    RESPONSES_MAP(F)               \
    LOBBY_COMMANDS_MAP(F)          \
    GAME_COMMANDS_MAP(F)           \
    UPDATES_MAP(F)

#define MESSAGE_TYPE_MAP_LAST(F) F(std::nullptr_t, NONE)
