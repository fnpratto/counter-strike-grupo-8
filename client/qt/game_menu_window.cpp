#include "game_menu_window.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "constants.h"

#define WINDOW_WIDTH 416
#define WINDOW_HEIGHT 307

GameMenuWindow::GameMenuWindow(Queue<Message>& input_queue, Queue<Message>& output_queue,
                               QWidget* parent):
        QWidget(parent), input_queue(input_queue), output_queue(output_queue) {
    this->setWindowTitle(TITLE);
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->init_gui();
}

GameMenuWindow::~GameMenuWindow() {}

void GameMenuWindow::init_gui() {
    this->main_layout = new QVBoxLayout(this);
    this->setLayout(this->main_layout);

    this->add_game_title();
    this->add_window_subtitle();
    this->add_game_list();
    this->add_start_buttons();
}

void GameMenuWindow::add_game_title() {
    QLabel* game_title_label = new QLabel(TITLE, this);
    game_title_label->setFixedHeight(32);
    game_title_label->setFont(QFont(CS_FONT_NAME, 32, QFont::Bold));
    game_title_label->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(game_title_label);
}

void GameMenuWindow::add_window_subtitle() {
    QLabel* window_subtitle_label = new QLabel("Game Menu", this);
    window_subtitle_label->setFixedHeight(16);
    window_subtitle_label->setFont(QFont(CS_FONT_NAME, 16, QFont::Bold));
    window_subtitle_label->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(window_subtitle_label);
}

void GameMenuWindow::add_game_list() {
    this->game_list_table = new GameListTable(this);
    this->main_layout->addWidget(this->game_list_table);
}

void GameMenuWindow::add_start_buttons() {
    QHBoxLayout* button_layout = new QHBoxLayout();
    this->main_layout->addLayout(button_layout);

    QPushButton* create_button = new QPushButton("Create Game", this);
    button_layout->addWidget(create_button);
    connect(create_button, &QPushButton::clicked, this, &GameMenuWindow::on_create_button_clicked);

    QPushButton* join_button = new QPushButton("Join Game", this);
    button_layout->addWidget(join_button);
    connect(join_button, &QPushButton::clicked, this, &GameMenuWindow::on_join_button_clicked);
}

void GameMenuWindow::on_create_button_clicked() {
    qDebug() << "Create Game button clicked";
    qDebug() << "Window width:" << this->width();
    qDebug() << "Window height:" << this->height();
}

void GameMenuWindow::on_join_button_clicked() {
    QString selected_game = this->game_list_table->get_selected_game();
    qDebug() << "Join Game button clicked. Selected game:" << selected_game;
    this->join_game(selected_game);
}

void GameMenuWindow::join_game(QString game_name) { qDebug() << "Joining game:" << game_name; }
