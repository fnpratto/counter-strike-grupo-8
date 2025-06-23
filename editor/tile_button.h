#ifndef TILE_BUTTON_H
#define TILE_BUTTON_H

#include <QPixmap>
#include <QPushButton>

#include "tile.h"

class TileButton: public QPushButton {
    Q_OBJECT
private:
    static TileButton* selected_tile_button;
    Tile tile;
    QPoint drag_start_position;
    bool is_dragging;

public:
    explicit TileButton(const Tile& tile, QWidget* parent = nullptr);

    static TileButton*& getSelectedTileButton();

    Tile get_tile() const;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    void on_toggled(bool checked);
};

#endif  // TILE_BUTTON_H
