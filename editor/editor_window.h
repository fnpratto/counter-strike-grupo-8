#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <QFileInfoList>
#include <QGridLayout>
#include <QPixmap>
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
    QFileInfoList get_tile_sheets_files(const QString& path);
    QPixmap get_tile_image(const QString& resource_path, const int& x, const int& y,
                           const int& size);
};

#endif  // EDITOR_WINDOW_H
