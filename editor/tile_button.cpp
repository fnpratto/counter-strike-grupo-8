#include "tile_button.h"

#include <QApplication>
#include <QDebug>
#include <QDrag>
#include <QFile>
#include <QIcon>
#include <QMimeData>
#include <QMouseEvent>
#include <QSize>
#include <QTextStream>

constexpr int TILE_SIZE = 32;

TileButton::TileButton(const Tile& tile, QWidget* parent):
        QPushButton(parent), tile(tile), drag_start_position(0, 0), is_dragging(false) {
    this->setIcon(QIcon(tile.image));
    this->setIconSize(QSize(TILE_SIZE, TILE_SIZE));
    this->setFixedSize(TILE_SIZE, TILE_SIZE);
    this->setCheckable(true);
    QFile file(":/styles/tile_button_style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        this->setStyleSheet(stream.readAll());
        file.close();
    }

    connect(this, &QPushButton::toggled, this, &TileButton::on_toggled);
}

TileButton*& TileButton::getSelectedTileButton() { return selected_tile_button; }

void TileButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        drag_start_position = event->pos();
    }
    QPushButton::mousePressEvent(event);
}

void TileButton::mouseReleaseEvent(QMouseEvent* event) {
    if (this->selected_tile_button == this) {
        this->selected_tile_button = nullptr;
    } else {
        if (this->selected_tile_button) {
            this->selected_tile_button->setChecked(false);
        }
        this->selected_tile_button = this;
    }

    QPushButton::mouseReleaseEvent(event);
}

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
    mime_data->setImageData(tile.image);
    drag->setMimeData(mime_data);
    drag->setPixmap(tile.image);
    drag->exec(Qt::CopyAction | Qt::MoveAction);

    QApplication::restoreOverrideCursor();
}

void TileButton::enterEvent(QEvent* event) {
    this->setIconSize(QSize(TILE_SIZE - 4, TILE_SIZE - 4));
    QPushButton::enterEvent(event);
}

void TileButton::leaveEvent(QEvent* event) {
    if (!this->isChecked()) {
        this->setIconSize(QSize(TILE_SIZE, TILE_SIZE));
    }

    QPushButton::leaveEvent(event);
}

void TileButton::on_toggled(bool checked) {
    if (checked) {
        this->setIconSize(QSize(TILE_SIZE - 4, TILE_SIZE - 4));
    } else {
        this->setIconSize(QSize(TILE_SIZE, TILE_SIZE));
    }
}
