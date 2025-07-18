#include "create_game_window.h"

#include <QMessageBox>
#include <vector>

#include "common/qt/constants.h"

#define WINDOW_WIDTH 416
#define WINDOW_HEIGHT 340

CreateGameWindow::CreateGameWindow(Queue<Message>& input_queue, Queue<Message>& output_queue,
                                   const std::string& player_name, QWidget* parent):
        QWidget(parent),
        input_queue(input_queue),
        output_queue(output_queue),
        player_name(player_name) {
    this->setWindowTitle(TITLE);
    this->setWindowIcon(QIcon(ICON_PATH));
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    output_queue.push(Message(ListMapsCommand()));
    map_names = input_queue.pop().get_content<ListMapsResponse>().get_map_names();

    this->init_gui();
}

CreateGameWindow::~CreateGameWindow() {}

void CreateGameWindow::init_gui() {
    this->main_layout = new QVBoxLayout(this);
    this->setLayout(this->main_layout);

    this->add_game_title();
    this->add_window_subtitle();
    this->add_game_name_input();
    this->add_map_list();
    this->add_create_button();
}

void CreateGameWindow::add_game_title() {
    QLabel* game_title = new QLabel(TITLE, this);
    game_title->setFont(QFont(CS_FONT_NAME, TITLE_FONT_SIZE, QFont::Bold));
    game_title->setFixedHeight(TITLE_FONT_SIZE);
    game_title->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(game_title);
}

void CreateGameWindow::add_window_subtitle() {
    QLabel* window_subtitle = new QLabel("Create Game", this);
    window_subtitle->setFont(QFont(CS_FONT_NAME, SUBTITLE_FONT_SIZE, QFont::Bold));
    window_subtitle->setFixedHeight(SUBTITLE_FONT_SIZE);
    window_subtitle->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(window_subtitle);
}

void CreateGameWindow::add_map_list() {
    QListWidget* map_list = new QListWidget(this);
    this->main_layout->addWidget(map_list);

    for (const auto& map: map_names) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(map));
        map_list->addItem(item);
    }
    map_list->setCurrentRow(0);
}

void CreateGameWindow::add_game_name_input() {
    QHBoxLayout* game_name_layout = new QHBoxLayout();
    QLabel* game_name_label = new QLabel("Game Name:", this);
    game_name_layout->addWidget(game_name_label);
    this->game_name_input = new QLineEdit(this);
    this->game_name_input->setPlaceholderText("Enter game name");
    game_name_layout->addWidget(this->game_name_input);
    this->main_layout->addLayout(game_name_layout);
}

void CreateGameWindow::add_create_button() {
    QPushButton* create_button = new QPushButton("Create Game", this);
    this->main_layout->addWidget(create_button);
    connect(create_button, &QPushButton::clicked, this,
            &CreateGameWindow::on_create_button_clicked);
}

void CreateGameWindow::on_create_button_clicked() {
    std::string game_name = this->game_name_input->text().toStdString();
    std::string map_name = this->findChild<QListWidget*>()->currentItem()->text().toStdString();

    output_queue.push(Message(CreateGameCommand(game_name, map_name, player_name)));
    auto response = input_queue.pop();

    switch (response.get_type()) {
        case MessageType::JOINED_GAME_RESP:
            this->close();
            break;

        case MessageType::ERROR_RESP:
            QMessageBox::warning(this, "Create Game Error",
                                 response.get_content<ErrorResponse>().get_error_message().c_str(),
                                 QMessageBox::Ok);
            break;

        default:
            QMessageBox::warning(this, "Create Game Error", "Unexpected response from server.",
                                 QMessageBox::Ok);
            break;
    }
}
