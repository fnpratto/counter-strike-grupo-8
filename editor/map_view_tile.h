#ifndef MAP_VIEW_TILE_H
#define MAP_VIEW_TILE_H

#include <QActionGroup>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QLabel>
#include <QWidget>
#include <map>

#include "tile.h"
#include "tile_button.h"

class MapViewTile: public QLabel {
    Q_OBJECT
    typedef void (MapViewTile::*ToolAction)();

private:
    Tile tile;
    QActionGroup* tool_group;
    std::map<QAction*, ToolAction> tool_actions;

signals:
    void ct_spawn_set();
    void ct_spawn_unset();
    void t_spawn_set();
    void t_spawn_unset();
    void bomb_site_set();
    void bomb_site_unset();

public:
    explicit MapViewTile(const int& row, const int& col, QActionGroup* tool_group,
                         QWidget* parent = nullptr);

    bool has_tile() const;
    Tile get_tile() const;
    void set_tile(const Tile& tile);
    void clear_tile();
    void clear_spawn_and_site();

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void init_tool_actions();

    void place_tile();
    void erase();
    void set_ct_spawn();
    void set_t_spawn();
    void set_bomb_site();

    void set_ct_spawn_style();
    void set_t_spawn_style();
    void set_bomb_site_style();
    void set_default_style();
};

#endif  // MAP_VIEW_TILE_H
