#include "map_view_tile.h"

#include <QDebug>
#include <QMimeData>

#include "tile_button.h"

constexpr int TILE_SIZE = 32;

MapViewTile::MapViewTile(const int& row, const int& col, QWidget* parent): QLabel(parent) {
    this->tile.x = row;
    this->tile.y = col;
    this->setFixedSize(TILE_SIZE, TILE_SIZE);
    this->setStyleSheet("background-color: #000000; border: 1px solid #ffeeaa;");
    this->setAlignment(Qt::AlignCenter);
    this->setScaledContents(true);
    this->setAcceptDrops(true);
}

bool MapViewTile::has_tile() const { return !this->tile.image.isNull(); }

Tile MapViewTile::get_tile() const { return this->tile; }

void MapViewTile::set_tile(const Tile& tile) {
    int x = this->tile.x;
    int y = this->tile.y;

    this->tile = tile;
    this->tile.x = x;
    this->tile.y = y;

    QPixmap tile_pixmap = this->tile.image;
    if (!tile_pixmap.isNull()) {
        tile_pixmap = tile_pixmap.scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
    }

    this->setPixmap(tile_pixmap);
}

void MapViewTile::clear_tile() {
    this->set_tile(Tile());
    this->setPixmap(QPixmap());
}

void MapViewTile::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-tile")) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MapViewTile::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat("application/x-tile")) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MapViewTile::dropEvent(QDropEvent* event) {
    if (event->mimeData()->hasFormat("application/x-tile")) {
        QByteArray tile_data = event->mimeData()->data("application/x-tile");
        QDataStream stream(&tile_data, QIODevice::ReadOnly);
        Tile new_tile;
        stream >> new_tile;
        this->set_tile(new_tile);
        event->accept();
    } else {
        event->ignore();
    }
}

void MapViewTile::mousePressEvent(QMouseEvent* event) { QLabel::mousePressEvent(event); }

void MapViewTile::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && TileButton::getSelectedTileButton()) {
        this->set_tile(TileButton::getSelectedTileButton()->get_tile());
    }
    QLabel::mouseReleaseEvent(event);
}
