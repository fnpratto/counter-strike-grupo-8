#include <QApplication>

#include "editor_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    EditorWindow window;
    window.show();
    return app.exec();
}
