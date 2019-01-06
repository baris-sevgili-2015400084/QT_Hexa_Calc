#include "maincalc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainCalc w;
    w.show();




    return a.exec();
}
