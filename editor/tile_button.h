#ifndef TILE_BUTTON_H
#define TILE_BUTTON_H

#include <QPixmap>
#include <QPushButton>

class TileButton: public QPushButton {
    Q_OBJECT
private:
    QPixmap tile_pixmap;
    QPoint drag_start_position;

public:
    explicit TileButton(const QPixmap& tile_pixmap, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

#endif  // TILE_BUTTON_H
