#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <QFileInfoList>
#include <QGridLayout>
#include <QLineEdit>
#include <QPixmap>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

#include "map_view_tile.h"

class EditorWindow: public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* main_layout;
    QLineEdit* map_name;
    QSpinBox* map_max_players;
    std::vector<MapViewTile*> map_view_tiles;

public:
    explicit EditorWindow(QWidget* parent = nullptr);

private:
    void init_gui();
    void add_sidebar();
    void add_map_view();
    void add_tool_bar(QVBoxLayout* sidebar_layout);
    void add_tile_bar(QVBoxLayout* sidebar_layout);
    void add_inputs(QVBoxLayout* sidebar_layout);
    void add_buttons(QVBoxLayout* sidebar_layout);
    void add_tiles(QGridLayout* tilebar_layout);
    QFileInfoList get_tile_sheets_files(const QString& path);
    QPixmap get_tile_image(const QString& resource_path, const int& x, const int& y,
                           const int& size);
    void add_map_name_input(QVBoxLayout* sidebar_layout);
    void add_map_max_players_input(QVBoxLayout* sidebar_layout);
    void save_map();
    QString get_save_path();
};

#endif  // EDITOR_WINDOW_H
