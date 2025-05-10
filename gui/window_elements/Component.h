#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "Area.h"
#include "SdlText.h"
#include "SdlTexture.h"
#include "SdlWindow.h"

class Component {
public:
    Component(SdlTexture texture, SdlText text, Area size, Area dest);

    void render(SdlWindow& window) const;

    void setText(const SdlText& newText);
    void setSize(const Area& newSize);
    void setDest(const Area& newDest);

    const SdlTexture& getTexture() const;
    const SdlText& getText() const;
    const Area& getSize() const;
    const Area& getDest() const;

private:
    SdlTexture texture;
    SdlText text;
    Area size;
    Area dest;
};

#endif  // UICOMPONENT_H
