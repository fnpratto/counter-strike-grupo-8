#ifndef SHOP_DISPLAY_H
#define SHOP_DISPLAY_H

#include <optional>
#include <string>
#include <vector>

#include "../../game_config.h"
#include "../window_elements/area.h"
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/message.h"

class shopDisplay {
public:
    explicit shopDisplay(SdlWindow& window, const GameUpdate& state);
    void render();
    std::optional<Message> getPurchaseCommand(int x, int y);
    void updateShopState(bool state);
    void updatePrices(const ShopPricesResponse& response);
    ItemSlot get_ammo_type(int id_slot);
    bool isActive();

private:
    SdlWindow& window;
    const GameUpdate& game_state;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    SdlTexture gun_icons;
    SdlTexture ammo_icons;
    SdlText cost_money;
    SdlTexture back;
    SdlTexture back_chosen;
    SdlText gunNumber;
    int gun_buy;
    int size_guns_w;
    int size_guns_h;
    int size_slots_w;
    int size_slots_h;
    bool active;
    std::vector<GameConfig::GunInfo> guns;   // Moved guns into the class
    std::vector<GameConfig::AmmoInfo> ammo;  // Moved ammo into the class
    void renderSlots();
    void renderItem();
};
#endif  // SHOP_DISPLAY_H
