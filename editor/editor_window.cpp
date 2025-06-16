#include "editor_window.h"

#include <QAction>
#include <QDebug>
#include <QFontDatabase>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <array>

#include "common/qt/constants.h"

#include "map_view_tile.h"
#include "tile_button.h"

#define MAX_COLUMNS_TILEBAR 5
#define MAX_COLUMNS_MAPVIEW 20
#define MAX_ROWS_MAPVIEW 20
#define TILE_SIZE 32

constexpr std::array<int, 11> DUST_SKIP_TILES = {0, 39, 55, 64, 65, 68, 69, 70, 71, 72, 73};
constexpr std::array<int, 5> AZTEC_SKIP_TILES = {0, 46, 47, 48, 49};

EditorWindow::EditorWindow(QWidget* parent): QWidget(parent) {
    this->setWindowTitle(EDITOR_TITLE);
    this->setWindowIcon(QIcon(ICON_PATH));
    // this->setFixedSize(800, 600);
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
            MapViewTile* empty_tile = new MapViewTile(this);
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

    // Add actions to the toolbar
    toolbar->addAction(QIcon::fromTheme("document-new"), "New");
    toolbar->addAction(QIcon::fromTheme("document-open"), "Open");
    toolbar->addAction(QIcon::fromTheme("document-save"), "Save");

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

    this->add_dust_tiles(tilebar_layout, row, col);
    this->add_aztec_tiles(tilebar_layout, row, col);
}

void EditorWindow::add_dust_tiles(QGridLayout* tilebar_layout, int& row, int& col) {
    QPixmap dust_tile_image(":/resources/dust.bmp");
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 8; ++j) {
            int tile_index = i * 8 + j;
            if (std::find(DUST_SKIP_TILES.begin(), DUST_SKIP_TILES.end(), tile_index) !=
                DUST_SKIP_TILES.end()) {
                continue;
            }
            QPixmap tile = dust_tile_image.copy(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
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

void EditorWindow::add_aztec_tiles(QGridLayout* tilebar_layout, int& row, int& col) {
    QPixmap aztec_tile_image(":/resources/default_aztec.png");
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 5; ++j) {
            int tile_index = i * 5 + j;
            if (std::find(AZTEC_SKIP_TILES.begin(), AZTEC_SKIP_TILES.end(), tile_index) !=
                AZTEC_SKIP_TILES.end()) {
                continue;
            }
            QPixmap tile =
                    aztec_tile_image.copy(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
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
