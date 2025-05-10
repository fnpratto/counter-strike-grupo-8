#include "log_in_window.h"

#include <QFontDatabase>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <string>

// #define FONT_PATH ":/resources/counter_strike.ttf"
// #define ICON_PATH ":/resources/cs2d.bmp"
#define FONT_PATH "assets/gfx/fonts/counter_strike.ttf"
#define ICON_PATH "assets/gfx/cs2d.bmp"

LogInWindow::LogInWindow(int width, int height): QWidget(nullptr) {
    this->setWindowTitle("Counter Strike 2D");
    this->setWindowIcon(QIcon(ICON_PATH));
    this->setFixedSize(width, height);
    QFontDatabase::addApplicationFont(FONT_PATH);
    this->init_gui();
    // this->center_window();
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
    game_title->setFont(QFont("Counter-Strike", 32, QFont::Bold));
    game_title->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(game_title);
}

void LogInWindow::add_ip_input() {
    QHBoxLayout* ip_layout = new QHBoxLayout();
    QLabel* ip_label = new QLabel("IP:", this);
    ip_layout->addWidget(ip_label);
    this->ip_input = new QLineEdit(this);
    ip_layout->addWidget(this->ip_input);
    this->main_layout->addLayout(ip_layout);
}

void LogInWindow::add_port_input() {
    QHBoxLayout* port_layout = new QHBoxLayout();
    QLabel* port_label = new QLabel("Port:", this);
    port_layout->addWidget(port_label);
    this->port_input = new QLineEdit(this);
    port_layout->addWidget(this->port_input);
    this->main_layout->addLayout(port_layout);
}

void LogInWindow::add_login_button() {
    QPushButton* login_button = new QPushButton("Login", this);
    this->main_layout->addWidget(login_button);
    connect(login_button, &QPushButton::clicked, this, &LogInWindow::on_login_button_clicked);
}
void LogInWindow::on_login_button_clicked() {
    std::string ip = this->ip_input->text().toStdString();
    std::string port = this->port_input->text().toStdString();

    if (ip.empty() || port.empty()) {
        QMessageBox::warning(this, "Input Error", "IP and Port cannot be empty.", QMessageBox::Ok);
        return;
    }
}
