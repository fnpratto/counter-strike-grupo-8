#ifndef HUD_DISPLAY_H
#define HUD_DISPLAY_H

#include "../window_elements/Area.h"  // Assuming Area is in this header
#include "../window_elements/Component.h"
#include "../window_elements/SdlText.h"
#include "../window_elements/SdlTexture.h"
#include "../window_elements/SdlWindow.h"

class hudDisplay {
public:
    hudDisplay(SdlWindow& window);
    void render();

private:
    SdlWindow& window;
    SdlTexture back;
    SdlTexture pointer;
    Component moneyComponent;
    Component lifeComponent;
    Component equipedGunsComponent;
    Component bulletsComponent;
    Component chronometerComponent;
};

#endif  // HUD_DISPLAY_H
