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
    void add_tool_bar(QVBoxLayout* sidebar_layout);
    void add_tile_bar(QVBoxLayout* sidebar_layout);
    void add_buttons(QVBoxLayout* sidebar_layout);
    void add_tiles(QGridLayout* tilebar_layout);

    template <size_t N>
    void add_some_tiles(QGridLayout* tilebar_layout, int& row, int& col, const QPixmap& tile_image,
                        int tile_cols, int tile_rows, const std::array<int, N>& skip_tiles);
};

#endif  // EDITOR_WINDOW_H
