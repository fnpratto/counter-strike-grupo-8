#include "map_view_tile.h"

#include <QDebug>
#include <QMimeData>

#include "tile_button.h"

constexpr int TILE_SIZE = 32;

MapViewTile::MapViewTile(const int& row, const int& col, QActionGroup* tool_group, QWidget* parent):
        QLabel(parent), tool_group(tool_group) {
    this->tile.x = row;
    this->tile.y = col;
    this->setFixedSize(TILE_SIZE, TILE_SIZE);
    this->setStyleSheet("background-color: #000000; border: 1px solid #ffeeaa;");
    this->setAlignment(Qt::AlignCenter);
    this->setScaledContents(true);
    this->setAcceptDrops(true);
    this->init_tool_actions();
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

    if (this->tile.is_ct_spawn) {
        this->set_ct_spawn_style();
    } else if (this->tile.is_t_spawn) {
        this->set_t_spawn_style();
    } else if (this->tile.is_bomb_site) {
        this->set_bomb_site_style();
    } else {
        this->set_default_style();
    }
}

void MapViewTile::clear_tile() {
    this->set_tile(Tile());
    this->setPixmap(QPixmap());
}

void MapViewTile::clear_spawn_and_site() {
    this->tile.is_ct_spawn = false;
    this->tile.is_t_spawn = false;
    this->tile.is_bomb_site = false;
    this->set_default_style();
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
        new_tile.is_ct_spawn = this->tile.is_ct_spawn;
        new_tile.is_t_spawn = this->tile.is_t_spawn;
        new_tile.is_bomb_site = this->tile.is_bomb_site;
        this->set_tile(new_tile);
        event->accept();
    } else {
        event->ignore();
    }
}

void MapViewTile::mousePressEvent(QMouseEvent* event) { QLabel::mousePressEvent(event); }

void MapViewTile::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        (this->*tool_actions.at(this->tool_group->checkedAction()))();
    }
    QLabel::mouseReleaseEvent(event);
}

void MapViewTile::init_tool_actions() {
    this->tool_actions.clear();
    this->tool_actions[this->tool_group->actions().at(0)] = &MapViewTile::place_tile;
    this->tool_actions[this->tool_group->actions().at(1)] = &MapViewTile::erase;
    this->tool_actions[this->tool_group->actions().at(2)] = &MapViewTile::set_ct_spawn;
    this->tool_actions[this->tool_group->actions().at(3)] = &MapViewTile::set_t_spawn;
    this->tool_actions[this->tool_group->actions().at(4)] = &MapViewTile::set_bomb_site;
}

void MapViewTile::place_tile() {
    if (TileButton::getSelectedTileButton()) {
        Tile selected_tile = TileButton::getSelectedTileButton()->get_tile();
        selected_tile.is_ct_spawn = this->tile.is_ct_spawn;
        selected_tile.is_t_spawn = this->tile.is_t_spawn;
        selected_tile.is_bomb_site = this->tile.is_bomb_site;
        this->set_tile(selected_tile);
    }
}

void MapViewTile::erase() {
    this->clear_tile();
    this->clear_spawn_and_site();
}

void MapViewTile::set_ct_spawn() {
    this->tile.is_ct_spawn = !this->tile.is_ct_spawn;
    this->tile.is_t_spawn = false;
    this->tile.is_bomb_site = false;

    if (this->tile.is_ct_spawn) {
        this->set_ct_spawn_style();
    } else {
        this->set_default_style();
    }
}

void MapViewTile::set_t_spawn() {
    this->tile.is_t_spawn = !this->tile.is_t_spawn;
    this->tile.is_ct_spawn = false;
    this->tile.is_bomb_site = false;

    if (this->tile.is_t_spawn) {
        this->set_t_spawn_style();
    } else {
        this->set_default_style();
    }
}

void MapViewTile::set_bomb_site() {
    this->tile.is_bomb_site = !this->tile.is_bomb_site;
    this->tile.is_ct_spawn = false;
    this->tile.is_t_spawn = false;

    if (this->tile.is_bomb_site) {
        this->set_bomb_site_style();
    } else {
        this->set_default_style();
    }
}

void MapViewTile::set_ct_spawn_style() {
    this->setStyleSheet("background-color: #000000; border: 1px solid #5d79ae;");
}

void MapViewTile::set_t_spawn_style() {
    this->setStyleSheet("background-color: #000000; border: 1px solid #de9b35;");
}

void MapViewTile::set_bomb_site_style() {
    this->setStyleSheet("background-color: #000000; border: 1px solid #ff0000;");
}

void MapViewTile::set_default_style() {
    this->setStyleSheet("background-color: #000000; border: 1px solid #ffeeaa;");
}
