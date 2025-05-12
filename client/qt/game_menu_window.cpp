#include "game_menu_window.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "constants.h"

GameMenu::GameMenu(QWidget* parent): QWidget(parent) {
    this->setWindowTitle(TITLE);
    // this->setFixedSize(800, 600);
    this->init_gui();
}

GameMenu::~GameMenu() {}

void GameMenu::init_gui() {
    this->main_layout = new QVBoxLayout(this);
    this->setLayout(this->main_layout);

    this->add_game_title();
    this->add_window_subtitle();
    this->add_game_list();
    this->add_start_buttons();
}

void GameMenu::add_game_title() {
    QLabel* game_title = new QLabel(TITLE, this);
    game_title->setFont(QFont(CS_FONT_NAME, 32, QFont::Bold));
    game_title->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(game_title);
}

void GameMenu::add_window_subtitle() {
    QLabel* window_subtitle = new QLabel("Game Menu", this);
    window_subtitle->setFont(QFont(CS_FONT_NAME, 16, QFont::Bold));
    window_subtitle->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(window_subtitle);
}

void GameMenu::add_game_list() {
    this->game_list = new GameListTable(this);
    this->main_layout->addWidget(this->game_list);
}

void GameMenu::add_start_buttons() {
    QHBoxLayout* button_layout = new QHBoxLayout();
    this->main_layout->addLayout(button_layout);

    QPushButton* create_button = new QPushButton("Create Game", this);
    button_layout->addWidget(create_button);

    QPushButton* join_button = new QPushButton("Join Game", this);
    button_layout->addWidget(join_button);
}
