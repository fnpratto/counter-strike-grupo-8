#ifndef TILE_H
#define TILE_H

#include <QPixmap>

struct Tile {
    int id;
    QPixmap image;
    bool collidable;
};

#endif  // TILE_H
