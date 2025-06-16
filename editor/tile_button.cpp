#include "tile_button.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

constexpr int TILE_SIZE = 32;

TileButton::TileButton(const QPixmap& tile_pixmap, QWidget* parent):
        QPushButton(parent), tile_pixmap(tile_pixmap) {
    this->setIcon(QIcon(tile_pixmap));
    this->setIconSize(QSize(TILE_SIZE, TILE_SIZE));
    this->setFixedSize(TILE_SIZE, TILE_SIZE);
}

void TileButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        drag_start_position = event->pos();
    }
    QPushButton::mousePressEvent(event);
}

void TileButton::mouseReleaseEvent(QMouseEvent* event) { QPushButton::mouseReleaseEvent(event); }

void TileButton::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - drag_start_position).manhattanLength() <
        QApplication::startDragDistance()) {
        return;
    }

    QApplication::setOverrideCursor(Qt::DragMoveCursor);

    QDrag* drag = new QDrag(this);
    QMimeData* mime_data = new QMimeData();
    mime_data->setImageData(tile_pixmap);
    drag->setMimeData(mime_data);
    drag->setPixmap(tile_pixmap);
    drag->exec(Qt::CopyAction | Qt::MoveAction);

    QApplication::restoreOverrideCursor();
}
