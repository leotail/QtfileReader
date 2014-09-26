#include "ms1.h"
#include <QApplication>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    MS1 w;
    w.show();

    return a.exec();
}
