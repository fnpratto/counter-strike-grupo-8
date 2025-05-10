#include "Component.h"


Component::Component(SdlTexture texture, SdlText text, Area size, Area dest):
        texture(std::move(texture)), text(std::move(text)), size(size), dest(dest) {}

void Component::render(SdlWindow& window) const {

    texture.render(size, dest);

    text.render(dest);
}

void Component::setText(const SdlText& newText) { text = newText; }

void Component::setSize(const Area& newSize) { size = newSize; }

void Component::setDest(const Area& newDest) { dest = newDest; }

const SdlTexture& Component::getTexture() const { return texture; }

const SdlText& Component::getText() const { return text; }

const Area& Component::getSize() const { return size; }

const Area& Component::getDest() const { return dest; }
