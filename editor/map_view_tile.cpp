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
        stream >> this->tile;
        this->setPixmap(this->tile.image.scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
        event->accept();
    } else {
        event->ignore();
    }
}

void MapViewTile::mousePressEvent(QMouseEvent* event) { QLabel::mousePressEvent(event); }

void MapViewTile::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && TileButton::getSelectedTileButton()) {
        this->set_tile(TileButton::getSelectedTileButton()->get_tile());
        QPixmap pixmap = this->tile.image.scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation);
        this->setPixmap(pixmap);
    }
    QLabel::mouseReleaseEvent(event);
}
