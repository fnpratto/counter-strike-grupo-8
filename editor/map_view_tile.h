#ifndef MAP_VIEW_TILE_H
#define MAP_VIEW_TILE_H

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QLabel>
#include <QWidget>

#include "tile.h"
#include "tile_button.h"

class MapViewTile: public QLabel {
    Q_OBJECT
private:
    Tile tile;

public:
    explicit MapViewTile(const int& row, const int& col, QWidget* parent = nullptr);

    bool has_tile() const;
    Tile get_tile() const;
    void set_tile(const Tile& tile);
    void clear_tile();

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif  // MAP_VIEW_TILE_H
