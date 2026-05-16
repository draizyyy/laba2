#include <QApplication>
#include "SequenceWindow.hpp"  

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SequenceWindow window; 
    window.show();
    return app.exec();
}