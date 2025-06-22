#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <QFileInfoList>
#include <QGridLayout>
#include <QLineEdit>
#include <QPixmap>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "map_view_tile.h"

constexpr int MAX_ROWS_MAPVIEW = 20;
constexpr int MAX_COLS_MAPVIEW = 20;

class EditorWindow: public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* main_layout;
    QGridLayout* tile_buttons_layout;
    QLineEdit* map_name;
    QSpinBox* map_max_players;
    QGridLayout* map_view_layout;
    QActionGroup* tool_group;
    int ct_spawn_count = 0;
    int t_spawn_count = 0;
    int bomb_site_count = 0;

public:
    explicit EditorWindow(QWidget* parent = nullptr);

private:
    void init_gui();

    void add_sidebar();
    void add_map_view();

    void add_tool_bar(QVBoxLayout* sidebar_layout);
    void add_tile_buttons_layout(QVBoxLayout* sidebar_layout);
    void add_inputs(QVBoxLayout* sidebar_layout);
    void add_buttons(QVBoxLayout* sidebar_layout);

    void add_tile_buttons();
    QFileInfoList get_tile_sheets_files(const QString& path);
    QPixmap get_tile_image(const QString& resource_path, const int& x, const int& y,
                           const int& size);

    void add_map_name_input(QVBoxLayout* sidebar_layout);
    void add_map_max_players_input(QVBoxLayout* sidebar_layout);

    void save_map();
    bool is_map_valid();
    QString get_save_path();
    void write_yaml(YAML::Node& map_data);

    void open_map();
    void read_yaml(const YAML::Node& map_data);
    void read_map_error_dialog(const std::string& error_message);
    void clear_map_view();
    QPixmap find_tile_image_by_id(const int& id);
};

#endif  // EDITOR_WINDOW_H
