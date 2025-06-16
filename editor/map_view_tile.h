#ifndef MAP_VIEW_TILE_H
#define MAP_VIEW_TILE_H

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QLabel>
#include <QWidget>

class MapViewTile: public QLabel {
    Q_OBJECT

public:
    explicit MapViewTile(QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
};

#endif  // MAP_VIEW_TILE_H
