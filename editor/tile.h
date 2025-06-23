#ifndef TILE_H
#define TILE_H

#include <QDataStream>
#include <QPixmap>

struct Tile {
    int id = -1;
    QPixmap image;
    int x = -1;
    int y = -1;
    bool is_collidable = false;
    bool is_ct_spawn = false;
    bool is_t_spawn = false;
    bool is_bomb_site = false;

    friend QDataStream& operator<<(QDataStream& out, const Tile& tile) {
        out << tile.id << tile.image << tile.is_collidable << tile.is_ct_spawn << tile.is_t_spawn
            << tile.is_bomb_site;
        return out;
    }

    friend QDataStream& operator>>(QDataStream& in, Tile& tile) {
        in >> tile.id >> tile.image >> tile.is_collidable >> tile.is_ct_spawn >> tile.is_t_spawn >>
                tile.is_bomb_site;
        return in;
    }
};

#endif  // TILE_H
