#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>

class EditorWindow: public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* main_layout;

public:
    explicit EditorWindow(QWidget* parent = nullptr);

private:
    void init_gui();
    void add_sidebar();
    void add_map_view();
    void add_toolbar(QVBoxLayout* sidebar_layout);
    void add_tilebar(QVBoxLayout* sidebar_layout);
    void add_buttons(QVBoxLayout* sidebar_layout);
    void add_tiles(QGridLayout* tilebar_layout);
    void add_dust_tiles(QGridLayout* tilebar_layout, int& row, int& col);
    void add_aztec_tiles(QGridLayout* tilebar_layout, int& row, int& col);
};

#endif  // EDITOR_WINDOW_H
