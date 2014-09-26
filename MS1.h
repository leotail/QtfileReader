#ifndef MS1_H
#define MS1_H
#include <QDebug>
#include <QStandardItemModel>
#include <QList>
#include <QStandardItem>
#include <QMainWindow>
#include <QxtCsvModel>
#include <QItemSelectionModel>


namespace Ui {
class MS1;
}

class MS1 : public QMainWindow
{
    Q_OBJECT

public:

    explicit MS1(QWidget *parent = 0);
    ~MS1();

private slots:
    double findMax(QList<double>);
    double findMin(QList<double>);
    void plotLineGraph(QxtCsvModel *CModel);
    void on_actionUpdate_triggered();
    void on_actionSave_triggered();
    void on_actionDelete_rows_triggered();
    void on_actionExit_triggered();



private:
    Ui::MS1 *ui;
    QList<double> listX;
    QList<double> listY;
    QxtCsvModel *csvmodel;
    QString fileName;
    QString showTime;
    static bool useFile;
    //QItemSelectionModel *selectedModel;
    QModelIndexList selectedIndexes;
};

#endif // MS1_H
