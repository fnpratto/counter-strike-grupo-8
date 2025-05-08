#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "display.h"

class TextDisplay: public Display {
public:
    void draw(const Message& message) override;
};
