#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(400, 300);
    window.setStyleSheet("background-color: yellow;");
    window.setMinimumSize(650,800);




    window.show();
    return app.exec();
}