#include "map_view_tile.h"

#include <QMimeData>

constexpr int TILE_SIZE = 32;

MapViewTile::MapViewTile(TileButton*& selected_tile_button, QWidget* parent):
        QLabel(parent), selected_tile_button(selected_tile_button) {
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

void MapViewTile::mousePressEvent(QMouseEvent* event) { QLabel::mousePressEvent(event); }

void MapViewTile::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && selected_tile_button) {
        QPixmap pixmap = selected_tile_button->icon()
                                 .pixmap(TILE_SIZE, TILE_SIZE)
                                 .scaled(TILE_SIZE, TILE_SIZE, Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
        this->setPixmap(pixmap);
    }
    QLabel::mouseReleaseEvent(event);
}
