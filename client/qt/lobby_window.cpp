#include "lobby_window.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <string>

#include "constants.h"

#define WINDOW_WIDTH 416
#define WINDOW_HEIGHT 307

LobbyWindow::LobbyWindow(Queue<Message>& input_queue, Queue<Message>& output_queue,
                         QWidget* parent):
        QWidget(parent), input_queue(input_queue), output_queue(output_queue) {
    this->setWindowTitle(TITLE);
    this->setWindowIcon(QIcon(ICON_PATH));
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->init_gui();
}

LobbyWindow::~LobbyWindow() {}

void LobbyWindow::init_gui() {
    this->main_layout = new QVBoxLayout(this);
    this->setLayout(this->main_layout);

    this->add_game_title();
    this->add_window_subtitle();
    this->add_player_name_input();
    this->add_game_list();
    this->add_refresh_button();
    this->add_start_buttons();

    this->update_game_list();
}

void LobbyWindow::add_game_title() {
    QLabel* game_title_label = new QLabel(TITLE, this);
    game_title_label->setFixedHeight(TITLE_FONT_SIZE);
    game_title_label->setFont(QFont(CS_FONT_NAME, TITLE_FONT_SIZE, QFont::Bold));
    game_title_label->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(game_title_label);
}

void LobbyWindow::add_window_subtitle() {
    // change class and file name
    QLabel* window_subtitle_label = new QLabel("Lobby", this);
    window_subtitle_label->setFixedHeight(SUBTITLE_FONT_SIZE);
    window_subtitle_label->setFont(QFont(CS_FONT_NAME, SUBTITLE_FONT_SIZE, QFont::Bold));
    window_subtitle_label->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(window_subtitle_label);
}

void LobbyWindow::add_game_list() {
    this->game_list_table = new GameListTable(this);
    this->main_layout->addWidget(this->game_list_table);
}

void LobbyWindow::add_refresh_button() {
    QPushButton* refresh_button = new QPushButton("Refresh Game List", this);
    refresh_button->setDefault(true);
    this->main_layout->addWidget(refresh_button);
    connect(refresh_button, &QPushButton::clicked, this, [this]() {
        this->update_game_list();
        qDebug() << "Game list refreshed.";
    });
}

void LobbyWindow::add_player_name_input() {
    QHBoxLayout* player_name_layout = new QHBoxLayout();
    QLabel* player_name_label = new QLabel("Player Name:", this);
    player_name_layout->addWidget(player_name_label);
    this->player_name_input = new QLineEdit(this);
    player_name_layout->addWidget(this->player_name_input);
    this->player_name_input->setPlaceholderText("Enter player name");
    this->main_layout->addLayout(player_name_layout);
}

void LobbyWindow::add_start_buttons() {
    QHBoxLayout* button_layout = new QHBoxLayout();
    this->main_layout->addLayout(button_layout);

    QPushButton* create_button = new QPushButton("Create Game", this);
    button_layout->addWidget(create_button);
    connect(create_button, &QPushButton::clicked, this, &LobbyWindow::on_create_button_clicked);

    QPushButton* join_button = new QPushButton("Join Game", this);
    button_layout->addWidget(join_button);
    connect(join_button, &QPushButton::clicked, this, &LobbyWindow::on_join_button_clicked);
}

void LobbyWindow::on_create_button_clicked() {
    if (player_name_input->text().isEmpty()) {
        this->player_name_empty_warning();
        return;
    }

    std::string player_name = this->player_name_input->text().toStdString();
    this->close();
    this->create_game_window = new CreateGameWindow(input_queue, output_queue, player_name);
    this->create_game_window->show();
}

void LobbyWindow::on_join_button_clicked() {
    QString selected_game = this->game_list_table->get_selected_game();

    if (selected_game.isEmpty()) {
        return;
    }

    qDebug() << "Join Game button clicked. Selected game:" << selected_game;
    this->join_game(selected_game);
}

void LobbyWindow::join_game(QString game_name) {
    std::string game_name_str = game_name.toStdString();

    if (player_name_input->text().isEmpty()) {
        this->player_name_empty_warning();
        return;
    }

    std::string player_name = this->player_name_input->text().toStdString();
    output_queue.push(Message(JoinGameCommand(game_name_str, player_name)));

    auto msg = input_queue.pop();
    while (msg.get_type() != MessageType::BOOL) {
        msg = input_queue.pop();
    }

    bool join_res = msg.get_content<bool>();
    if (!join_res) {
        QMessageBox::warning(this, "Join Game Error", "Failed to join game.", QMessageBox::Ok);
        return;
    }

    this->close();
}

void LobbyWindow::update_game_list() {
    output_queue.push(Message(ListGamesCommand()));
    auto msg = input_queue.pop();
    while (msg.get_type() != MessageType::LIST_GAMES_RESP) {
        msg = input_queue.pop();
    }
    auto game_info_list = msg.get_content<ListGamesResponse>().get_games_info();
    this->game_list_table->update_game_list(game_info_list);
}

void LobbyWindow::player_name_empty_warning() {
    QMessageBox::warning(this, "Warning", "Player name cannot be empty.", QMessageBox::Ok);
}
