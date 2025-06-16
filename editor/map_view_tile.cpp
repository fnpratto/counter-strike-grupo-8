#include "map_view_tile.h"

#include <QMimeData>

constexpr int TILE_SIZE = 32;

MapViewTile::MapViewTile(QWidget* parent): QLabel(parent) {
    this->setFixedSize(TILE_SIZE, TILE_SIZE);
    this->setStyleSheet("background-color: #000000; border: 1px solid #ffeeaa;");
    this->setAlignment(Qt::AlignCenter);
    this->setScaledContents(true);
    this->setAcceptDrops(true);
}

void MapViewTile::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasImage()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MapViewTile::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasImage()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MapViewTile::dropEvent(QDropEvent* event) {
    if (event->mimeData()->hasImage()) {
        QPixmap pixmap = qvariant_cast<QPixmap>(event->mimeData()->imageData());
        this->setPixmap(
                pixmap.scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        event->accept();
    } else {
        event->ignore();
    }
}
