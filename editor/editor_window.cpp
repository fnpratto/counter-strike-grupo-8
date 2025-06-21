#include "editor_window.h"

#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFontDatabase>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QStringList>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <array>
#include <fstream>
#include <string>

#include <yaml-cpp/yaml.h>

#include "common/qt/constants.h"

#include "map_view_tile.h"
#include "tile.h"
#include "tile_button.h"

constexpr int WINDOW_WIDTH = 845;
constexpr int WINDOW_HEIGHT = 662;

constexpr int MAX_COLUMNS_TILEBAR = 5;
constexpr int MAX_COLUMNS_MAPVIEW = 20;

TileButton* TileButton::selected_tile_button = nullptr;

EditorWindow::EditorWindow(QWidget* parent): QWidget(parent) {
    this->setWindowTitle(EDITOR_TITLE);
    this->setWindowIcon(QIcon(ICON_PATH));
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    QFontDatabase::addApplicationFont(CS_FONT_PATH);
    this->setStyleSheet(
            "QWidget { background-color: #404040; } QAction { border: 1px solid #ccc; }");
    this->init_gui();
}

void EditorWindow::init_gui() {
    this->main_layout = new QHBoxLayout(this);
    this->setLayout(this->main_layout);

    this->add_sidebar();
    this->add_map_view();
}

void EditorWindow::add_sidebar() {
    QVBoxLayout* sidebar_layout = new QVBoxLayout();
    sidebar_layout->setContentsMargins(0, 0, 0, 0);
    sidebar_layout->setSpacing(5);
    this->main_layout->addLayout(sidebar_layout);

    this->add_tool_bar(sidebar_layout);
    this->add_tile_buttons_layout(sidebar_layout);
    this->add_inputs(sidebar_layout);
    this->add_buttons(sidebar_layout);
}

void EditorWindow::add_map_view() {
    this->map_view_layout = new QGridLayout();
    this->map_view_layout->setContentsMargins(0, 0, 0, 0);
    this->map_view_layout->setSpacing(0);

    for (int row = 0; row < MAX_ROWS_MAPVIEW; ++row) {
        for (int col = 0; col < MAX_COLUMNS_MAPVIEW; ++col) {
            MapViewTile* empty_tile = new MapViewTile(row, col, this);
            this->map_view_layout->addWidget(empty_tile, row, col);
        }
    }

    this->main_layout->addLayout(this->map_view_layout);
}

void EditorWindow::add_tool_bar(QVBoxLayout* sidebar_layout) {
    QToolBar* toolbar = new QToolBar("Toolbar", this);
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(24, 24));
    // toolbar->setStyleSheet("QToolBar { border: 1px solid #ccc; }");

    QActionGroup* action_group = new QActionGroup(this);
    action_group->setExclusive(true);

    QAction* pencil = toolbar->addAction(QIcon::fromTheme("gtk-edit"), "Pencil");
    pencil->setCheckable(true);
    pencil->setChecked(true);
    action_group->addAction(pencil);
    QAction* rectangle = toolbar->addAction(QIcon::fromTheme("draw-rectangle"), "Rectangle");
    rectangle->setCheckable(true);
    action_group->addAction(rectangle);
    QAction* eraser = toolbar->addAction(QIcon::fromTheme("draw-eraser"), "Eraser");
    eraser->setCheckable(true);
    action_group->addAction(eraser);

    sidebar_layout->addWidget(toolbar);
}

void EditorWindow::add_tile_buttons_layout(QVBoxLayout* sidebar_layout) {
    QScrollArea* tile_buttons_scroll_area = new QScrollArea();
    sidebar_layout->addWidget(tile_buttons_scroll_area);

    QWidget* tilebar_widget = new QWidget();
    tile_buttons_scroll_area->setWidget(tilebar_widget);

    tile_buttons_scroll_area->setWidgetResizable(true);
    tile_buttons_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tile_buttons_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->tile_buttons_layout = new QGridLayout();
    this->tile_buttons_layout->setContentsMargins(0, 0, 15, 0);
    this->tile_buttons_layout->setSpacing(0);
    this->add_tile_buttons();
    tilebar_widget->setLayout(this->tile_buttons_layout);
}

void EditorWindow::add_inputs(QVBoxLayout* sidebar_layout) {
    this->add_map_name_input(sidebar_layout);
    this->add_map_max_players_input(sidebar_layout);
}

void EditorWindow::add_buttons(QVBoxLayout* sidebar_layout) {
    QHBoxLayout* button_layout = new QHBoxLayout();

    QPushButton* open_map_button = new QPushButton("Open Map", this);
    QPushButton* save_map_button = new QPushButton("Save Map", this);

    button_layout->addWidget(open_map_button);
    button_layout->addWidget(save_map_button);

    sidebar_layout->addLayout(button_layout);

    connect(open_map_button, &QPushButton::clicked, this, &EditorWindow::open_map);

    connect(save_map_button, &QPushButton::clicked, this, &EditorWindow::save_map);
}

void EditorWindow::add_tile_buttons() {
    int row = 0;
    int col = 0;

    QFileInfoList tile_sheets_files = this->get_tile_sheets_files("common/tile_sheets");

    if (tile_sheets_files.isEmpty()) {
        qDebug() << "No tile sheets found in common/tile_sheets directory.";
        return;
    }

    for (const QFileInfo& fileInfo: tile_sheets_files) {
        YAML::Node map_data = YAML::LoadFile(fileInfo.absoluteFilePath().toStdString());

        QString map_image_path = QString::fromStdString(map_data["path"].as<std::string>());
        int tile_size = map_data["tile_size"].as<int>();

        for (const auto& tile_data: map_data["tiles"]) {
            int tile_x = tile_data["x"].as<int>();
            int tile_y = tile_data["y"].as<int>();
            Tile tile;
            tile.id = tile_data["id"].as<int>();
            tile.image = this->get_tile_image(map_image_path, tile_x, tile_y, tile_size);
            tile.is_collidable = tile_data["collidable"].as<bool>();
            QPushButton* tile_button = new TileButton(tile, this);
            this->tile_buttons_layout->addWidget(tile_button, row, col);
            col++;
            if (col >= MAX_COLUMNS_TILEBAR) {
                col = 0;
                row++;
            }
        }
    }
}

QFileInfoList EditorWindow::get_tile_sheets_files(const QString& path) {
    QDir tile_sheets_dir(path);
    QStringList filters;
    filters << "*.yaml"
            << "*.yml";
    tile_sheets_dir.setNameFilters(filters);
    return tile_sheets_dir.entryInfoList(QDir::Files);
}

QPixmap EditorWindow::get_tile_image(const QString& resource_path, const int& x, const int& y,
                                     const int& size) {
    QPixmap tile_image(resource_path);
    return tile_image.copy(y * size, x * size, size, size);
}

void EditorWindow::add_map_name_input(QVBoxLayout* sidebar_layout) {
    QLabel* map_name_label = new QLabel("Map Name:", this);
    map_name_label->setStyleSheet("QLabel { color: #fff; }");
    this->map_name = new QLineEdit(this);
    this->map_name->setPlaceholderText("Enter map name...");
    this->map_name->setStyleSheet(
            "QLineEdit { background-color: #fff; border: 1px solid #ccc; border-radius: 2px; }");

    sidebar_layout->addWidget(map_name_label);
    sidebar_layout->addWidget(this->map_name);
}

void EditorWindow::add_map_max_players_input(QVBoxLayout* sidebar_layout) {
    QHBoxLayout* max_players_layout = new QHBoxLayout();

    QLabel* map_max_players_label = new QLabel("Max Players:", this);
    map_max_players_label->setStyleSheet("QLabel { color: #fff; }");

    this->map_max_players = new QSpinBox(this);
    this->map_max_players->setRange(2, 16);
    this->map_max_players->setValue(10);
    this->map_max_players->setSingleStep(2);
    this->map_max_players->setStyleSheet("QSpinBox { background-color: #ffffff; }");

    max_players_layout->addWidget(map_max_players_label);
    max_players_layout->addWidget(this->map_max_players);
    sidebar_layout->addLayout(max_players_layout);
}

void EditorWindow::save_map() {
    QString save_path = this->get_save_path();
    if (save_path.isEmpty()) {
        qDebug() << "Save path is empty. Map not saved.";
        return;
    }

    YAML::Node map_data;
    this->write_yaml(map_data);

    try {
        YAML::Emitter out;
        out << map_data;
        std::ofstream fout(save_path.toStdString());
        fout << out.c_str();
        fout.close();
        QMessageBox::information(this, "Map Saved", "Map saved successfully!");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Map Save Error", "Failed to save the map. Please try again.");
        return;
    }
}

QString EditorWindow::get_save_path() {
    QString save_path = QFileDialog::getSaveFileName(this, "Save Map", "", "YAML Files (*.yaml)");

    if (!save_path.isEmpty() && !save_path.endsWith(".yaml")) {
        save_path += ".yaml";
    }

    return save_path;
}

void EditorWindow::write_yaml(YAML::Node& map_data) {
    map_data["name"] = this->map_name->text().toStdString();
    map_data["max_players"] = this->map_max_players->value();
    map_data["height"] = MAX_ROWS_MAPVIEW;
    map_data["width"] = MAX_COLS_MAPVIEW;
    map_data["tiles"] = YAML::Node(YAML::NodeType::Sequence);

    int left = MAX_COLUMNS_MAPVIEW - 1;
    int top = MAX_ROWS_MAPVIEW - 1;
    int right = 0;
    int bottom = 0;

    for (int i = 0; i < MAX_ROWS_MAPVIEW; ++i) {
        for (int j = 0; j < MAX_COLUMNS_MAPVIEW; ++j) {
            const MapViewTile* map_view_tile = static_cast<MapViewTile*>(
                    this->map_view_layout->itemAtPosition(i, j)->widget());
            if (map_view_tile->has_tile()) {
                YAML::Node tile_data;
                Tile tile = map_view_tile->get_tile();
                tile_data["id"] = tile.id;
                tile_data["x"] = tile.x;
                tile_data["y"] = tile.y;
                tile_data["is_collidable"] = tile.is_collidable;
                tile_data["is_spawn_ct"] = tile.is_ct_spawn;
                tile_data["is_spawn_tt"] = tile.is_t_spawn;
                tile_data["is_bomb_site"] = tile.is_bomb_site;
                map_data["tiles"].push_back(tile_data);

                if (i < top) {
                    top = i;
                }
                if (i > bottom) {
                    bottom = i;
                }
                if (j < left) {
                    left = j;
                }
                if (j > right) {
                    right = j;
                }
            }
        }
    }

    int width = right - left + 1;
    int height = bottom - top + 1;
    map_data["height"] = height;
    map_data["width"] = width;
}

void EditorWindow::open_map() {
    QString open_path = QFileDialog::getOpenFileName(this, "Open Map", "", "YAML Files (*.yaml)");
    if (open_path.isEmpty()) {
        return;
    }

    try {
        YAML::Node map_data = YAML::LoadFile(open_path.toStdString());
        this->read_yaml(map_data);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Map Open Error", "Failed to open the map. Please try again.");
    }
}

void EditorWindow::read_yaml(const YAML::Node& map_data) {
    if (!map_data["max_players"].IsScalar() || !map_data["tiles"].IsSequence()) {
        this->read_map_error_dialog();
        return;
    }

    this->clear_map_view();

    for (const auto& tile_data: map_data["tiles"]) {
        if (tile_data.IsMap()) {
            int x = tile_data["x"].as<int>();
            int y = tile_data["y"].as<int>();

            Tile tile;
            tile.id = tile_data["id"].as<int>();
            tile.image = this->find_tile_image_by_id(tile.id);
            tile.is_collidable = tile_data["is_collidable"].as<bool>();
            tile.is_ct_spawn = tile_data["is_ct_spawn"].as<bool>();
            tile.is_t_spawn = tile_data["is_t_spawn"].as<bool>();
            tile.is_bomb_site = tile_data["is_bomb_site"].as<bool>();

            MapViewTile* map_view_tile = static_cast<MapViewTile*>(
                    this->map_view_layout->itemAtPosition(x, y)->widget());
            map_view_tile->set_tile(tile);
        } else {
            this->read_map_error_dialog();
            return;
        }
    }

    this->map_name->setText(QString::fromStdString(map_data["name"].as<std::string>()));
    this->map_max_players->setValue(map_data["max_players"].as<int>());
}

void EditorWindow::read_map_error_dialog() {
    QMessageBox::critical(this, "Map Read Error",
                          "Failed to read the map data. Please check the file format.");
}

void EditorWindow::clear_map_view() {
    for (int i = 0; i < MAX_ROWS_MAPVIEW; ++i) {
        for (int j = 0; j < MAX_COLUMNS_MAPVIEW; ++j) {
            MapViewTile* map_view_tile = static_cast<MapViewTile*>(
                    this->map_view_layout->itemAtPosition(i, j)->widget());
            map_view_tile->clear_tile();
        }
    }
}

QPixmap EditorWindow::find_tile_image_by_id(const int& id) {
    for (int i = 0; i < this->tile_buttons_layout->count(); ++i) {
        const TileButton* tile_button =
                static_cast<TileButton*>(this->tile_buttons_layout->itemAt(i)->widget());
        if (tile_button && tile_button->get_tile().id == id) {
            return tile_button->get_tile().image;
        }
    }

    return QPixmap();
}
