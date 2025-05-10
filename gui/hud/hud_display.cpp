#include "hud_display.h"

const Area sizeMoney = Area(0, 0, 100, 50);
const Area sizeChronometer = Area(0, 50, 100, 50);
const Area sizeLife = Area(0, 100, 100, 50);
const Area sizeEquipedGuns = Area(0, 150, 100, 50);
const Area sizeBullets = Area(0, 200, 100, 50);

const Area destMoney = Area(10, 10, 100, 50);
const Area destChronometer = Area(10, 70, 100, 50);
const Area destLife = Area(10, 130, 100, 50);
const Area destEquipedGuns = Area(10, 190, 100, 50);
const Area destBullets = Area(10, 250, 100, 50);
const Area sizePointer = Area(0, 0, 50, 50);
const Area destPointer = Area(10, 300, 50, 50);
const Area sizeBackground = Area(0, 0, 800, 600);
const Area destBackground = Area(0, 0, 800, 600);


hudDisplay::hudDisplay(SdlWindow& window):
        window(window),
        back("assets/gfx/background.bmp", window),
        pointer("assets/gfx/pointer.xcf", window),
        moneyComponent(SdlTexture("assets/gfx/money.xcf", window),
                       SdlText("assets/gfx/cs_regular.ttf", 20, {0xFF, 0xFF, 0xFF, 0xFF}, window),
                       sizeMoney, destMoney),
        lifeComponent(SdlTexture("assets/gfx/life.xcf", window),
                      SdlText("assets/gfx/cs_regular.ttf", 20, {0xFF, 0xFF, 0xFF, 0xFF}, window),
                      sizeLife, destLife),
        equipedGunsComponent(
                SdlTexture("assets/gfx/equiped_guns.xcf", window),
                SdlText("assets/gfx/cs_regular.ttf", 20, {0xFF, 0xFF, 0xFF, 0xFF}, window),
                sizeEquipedGuns, destEquipedGuns),
        bulletsComponent(SdlTexture("assets/gfx/bullets.xcf", window),
                         SdlText("assets/gfx/cs_regular.ttf", 20, {0xFF, 0xFF, 0xFF, 0xFF}, window),
                         sizeBullets, destBullets),
        chronometerComponent(
                SdlTexture("assets/gfx/chronometer.xcf", window),
                SdlText("assets/gfx/cs_regular.ttf", 20, {0xFF, 0xFF, 0xFF, 0xFF}, window),
                sizeChronometer, destChronometer) {}

void hudDisplay::render() {
    window.fill();  // fill background (gray)

    back.render(sizeBackground, destBackground);
    pointer.render(sizePointer, destPointer);

    moneyComponent.render(window);
    lifeComponent.render(window);
    equipedGunsComponent.render(window);
    bulletsComponent.render(window);
    chronometerComponent.render(window);


    window.render();  // swap buffers
}
