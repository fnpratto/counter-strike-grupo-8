#include "log_in_window.h"

#include <QDebug>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPoint>
#include <QPushButton>
#include <QScreen>
#include <string>

#include "constants.h"

LogInWindow::LogInWindow(QWidget* parent): QWidget(parent) {
    this->setWindowTitle(TITLE);
    this->setWindowIcon(QIcon(ICON_PATH));
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    QFontDatabase::addApplicationFont(CS_FONT_PATH);
    // this->center_window();  // not working
    this->init_gui();
}

void LogInWindow::center_window() {
    int screen_width = this->screen()->availableGeometry().width();
    int screen_height = this->screen()->availableGeometry().height();
    int x = (screen_width - WINDOW_WIDTH) / 2;
    int y = (screen_height - WINDOW_HEIGHT) / 2;
    this->move(QPoint(x, y));
    qDebug() << "Screen width:" << screen_width;
    qDebug() << "Screen height:" << screen_height;
    qDebug() << "Window width:" << WINDOW_WIDTH;
    qDebug() << "Window height:" << WINDOW_HEIGHT;
    qDebug() << "X:" << x;
    qDebug() << "Y:" << y;
    qDebug() << "Window position:" << this->pos();
    qDebug() << "Window size:" << this->size();
}

void LogInWindow::init_gui() {
    this->main_layout = new QVBoxLayout(this);
    this->setLayout(this->main_layout);

    this->add_game_title();
    this->add_window_subtitle();
    this->add_ip_input();
    this->add_port_input();
    this->add_login_button();
}

LogInWindow::~LogInWindow() {}

void LogInWindow::add_game_title() {
    QLabel* game_title = new QLabel(TITLE, this);
    game_title->setFont(QFont(CS_FONT_NAME, 32, QFont::Bold));
    game_title->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(game_title);
}

void LogInWindow::add_window_subtitle() {
    QLabel* window_title = new QLabel("Log In", this);
    window_title->setFont(QFont(CS_FONT_NAME, 16, QFont::Bold));
    window_title->setAlignment(Qt::AlignCenter);
    this->main_layout->addWidget(window_title);
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

    this->close();
    this->game_menu = new GameMenu();
    this->game_menu->show();
}
