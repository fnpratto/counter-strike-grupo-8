#ifndef MAP_VIEW_TILE_H
#define MAP_VIEW_TILE_H

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QLabel>
#include <QWidget>

#include "tile_button.h"

class MapViewTile: public QLabel {
    Q_OBJECT
private:
    TileButton*& selected_tile_button;

public:
    explicit MapViewTile(TileButton*& selected_tile_button, QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif  // MAP_VIEW_TILE_H
