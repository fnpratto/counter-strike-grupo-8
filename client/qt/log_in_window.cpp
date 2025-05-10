#include "log_in_window.h"

explicit LogInWindow::LogInWindow(QWidget* parent, int width, int height): QWidget(parent) {
    this->setWindowTitle("Counter Strike 2D");
    // this->center_window();
    this->setWindowIcon(QIcon(ICON_PATH));
    this->init_gui();
}

void LogInWindow::init_gui() {
    this->main_layout = new QVBoxLayout(this);
    this->setLayout(this->main_layout);
    this->add_game_title();
    this->add_ip_input();
    this->add_port_input();
    this->add_login_button();
}

LogInWindow::~LogInWindow() {}

void LogInWindow::add_game_title() {
    QLabel* game_title = new QLabel("Counter Strike 2D", this);
    game_title->setAlignment(Qt::AlignCenter);
    game_title->setFont(QFont(""));
    this->main_layout->addWidget(game_title);
}
