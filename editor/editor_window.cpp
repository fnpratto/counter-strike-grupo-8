#include "editor_window.h"

#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFontDatabase>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QStringList>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <array>
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
constexpr int MAX_ROWS_MAPVIEW = 20;

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
    this->add_tile_bar(sidebar_layout);
    this->add_buttons(sidebar_layout);
}

void EditorWindow::add_map_view() {
    QGridLayout* map_view_layout = new QGridLayout();
    map_view_layout->setContentsMargins(0, 0, 0, 0);
    map_view_layout->setSpacing(0);

    for (int row = 0; row < MAX_ROWS_MAPVIEW; ++row) {
        for (int col = 0; col < MAX_COLUMNS_MAPVIEW; ++col) {
            MapViewTile* empty_tile = new MapViewTile(TileButton::getSelectedTileButton(), this);
            map_view_layout->addWidget(empty_tile, row, col);
        }
    }

    this->main_layout->addLayout(map_view_layout);
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

void EditorWindow::add_tile_bar(QVBoxLayout* sidebar_layout) {
    QScrollArea* tilebar_scroll_area = new QScrollArea();
    sidebar_layout->addWidget(tilebar_scroll_area);

    QWidget* tilebar_widget = new QWidget();
    tilebar_scroll_area->setWidget(tilebar_widget);

    tilebar_scroll_area->setWidgetResizable(true);
    tilebar_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tilebar_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QGridLayout* tilebar_layout = new QGridLayout();
    tilebar_layout->setContentsMargins(0, 0, 15, 0);
    tilebar_layout->setSpacing(0);
    this->add_tiles(tilebar_layout);
    tilebar_widget->setLayout(tilebar_layout);
}

void EditorWindow::add_buttons(QVBoxLayout* sidebar_layout) {
    QHBoxLayout* button_layout = new QHBoxLayout();

    QPushButton* open_map_button = new QPushButton("Open Map", this);
    QPushButton* save_map_button = new QPushButton("Save Map", this);

    button_layout->addWidget(open_map_button);
    button_layout->addWidget(save_map_button);

    sidebar_layout->addLayout(button_layout);

    connect(open_map_button, &QPushButton::clicked, this,
            []() { qDebug() << "Open Map button clicked"; });

    connect(save_map_button, &QPushButton::clicked, this,
            []() { qDebug() << "Save Map button clicked"; });
}

void EditorWindow::add_tiles(QGridLayout* tilebar_layout) {
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
            tile.collidable = tile_data["collidable"].as<bool>();
            QPushButton* tile_button = new TileButton(tile, this);
            tilebar_layout->addWidget(tile_button, row, col);
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
