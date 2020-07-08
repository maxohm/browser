#include "catdir.h"
//
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    catdir w;
    if(w.init(
                a.arguments()))
        return a.exec();
    return 0;
}
