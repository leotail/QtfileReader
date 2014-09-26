#include "ms1.h"
#include "ui_ms1.h"
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <QxtCsvModel>
#include <QModelIndex>
#include <QtCore>
#include <QtGui>
#include <QDateTime>
#include <QMessageBox>
#include <qwt_plot_zoomer.h>


//
MS1::MS1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MS1)
{
    ui->setupUi(this);
}

MS1::~MS1()
{
    delete ui;

}

//check if a model has been read
bool MS1::useFile=false;

//Two help functions to set the range of x-axis and y-axis(count start from 0, so the lower bound of y-axis should be 0.)
double MS1::findMax(QList<double> maxList){
    double MaxL = maxList[0];
    for (int i=1; i<maxList.size(); i++)
    {
    if (maxList[i] > MaxL)
    MaxL = maxList[i];
    }
    return MaxL;
}

double MS1::findMin(QList<double> minList){
    double MinL = minList[0];
    for (int i=1; i<minList.size(); i++)
    {
    if (minList[i] < MinL)
    MinL = minList[i];
    }
    return MinL;
}



void MS1::plotLineGraph(QxtCsvModel *CModel){
    //Get the label of x-axis and y-axis
    QModelIndex xIndex = CModel->index(0,0);
    QVariant xLabel = (CModel->data(xIndex));
    QModelIndex yIndex = CModel->index(0,1);
    QVariant yLabel = (CModel->data(yIndex));


    //Set title and the style of the plot
    ui->qwtPlot->setTitle( "MVC Data Display" );
    ui->qwtPlot->setCanvasBackground(Qt::gray);


    QwtPlotCurve *curve = new QwtPlotCurve();

    //Title of this line if there are multiple lines
    //curve->setTitle( "" );
    //Set the line color and width
    curve->setPen( Qt::blue, 2,Qt::SolidLine );

    //Allow to zoom in and zoom out(Not good enough)
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );

    //Interface from the csvModel to the line plot
    for (int i=1;i<(csvmodel->rowCount());i++){
        QModelIndex xEnter = csvmodel->index(i,0);
        QVariant xData = (csvmodel->data(xEnter));
        listX.append(xData.toDouble());
    }
    for (int i=1;i<(csvmodel->rowCount());i++){
        QModelIndex yEnter = csvmodel->index(i,1);
        QVariant yData = (csvmodel->data(yEnter));
        listY.append(yData.toDouble());
    }
    //testList
    //listX << 0 << 2 << 3 << 4 << 5<< 7  << 8<< 10 <<11<<13;
    //listY << 17<< 16.5<< 8<< 3<< 5<< 7.5<< 9<< 10<< 12<< 14;

    //Set the x-axis and y-axis
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, xLabel.toString() );
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, findMin(listX)-5, findMax(listX)+5);
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, yLabel.toString());
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0.0, findMax(listY)+5);



    curve->setSamples(listX.toVector(), listY.toVector());
    curve->attach( ui->qwtPlot );

    ui->qwtPlot->resize( 300, 300 );
    ui->qwtPlot->replot();
    //empty the two QList for storing x, y axis data

    listX.clear();
    listY.clear();


    showTime = "The plot was updated on " + QDateTime::currentDateTime().toString(Qt::ISODate);
    ui->label->setText(showTime);
}


//read data from a data file, create a line graph of the data, allow editing
void MS1::on_actionUpdate_triggered()
{
        //delete the old plot
        ui->qwtPlot->detachItems( QwtPlotItem::Rtti_PlotCurve );
        ui->qwtPlot->detachItems( QwtPlotItem::Rtti_PlotMarker);
         //Read CSV file
        csvmodel=new QxtCsvModel(this);
        fileName = QFileDialog::getOpenFileName (this, "Open CSV file",QDir::currentPath(), "CSV (*.csv)");
        if (!fileName.isEmpty()){
        csvmodel->setSource(fileName);
        useFile=true;
        ui->label_2->setText(fileName);
        plotLineGraph(csvmodel);
        ui->tableView->setModel(csvmodel);



        //test number of columns and rows
        //QString test=QString::number(csvmodel->columnCount());
        //QString test=QString::number(csvmodel->rowCount());
        //QMessageBox::information(this,"test",test);

        //test for each entry in the table
        //QModelIndex tableIndex = csvmodel->index(0,1);
        //QVariant test = (csvmodel->data(tableIndex));
        //QMessageBox::information(this,"test",test.toString());
    }
}

//delete selected rows
void MS1::on_actionDelete_rows_triggered()
{
    if (useFile==true){
    //Get Selected rows
    selectedIndexes = ui->tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()){
        QMessageBox noSelectMessage;
        noSelectMessage.setText("Please select a row!");
        noSelectMessage.exec();
    }
    else{
    //Test selected, must remove from the end
    while (!selectedIndexes.isEmpty())
    {
        csvmodel->removeRows(selectedIndexes.last().row(), 1);
        selectedIndexes.removeLast();
    }

    }


   // if (selectedModel->hasSelection()){
    //csvmodel->removeRow(selectedIndex.row(), selectedIndex.parent());
    //}
   }
    else{
        QMessageBox noReadMessage;
        noReadMessage.setText("No file is read!");
        noReadMessage.exec();
    }
}



void MS1::on_actionSave_triggered()
{
    if (useFile==true){
    csvmodel->toCSV(fileName);
    QMessageBox haveSaved;

    //delete the old plot
    ui->qwtPlot->detachItems( QwtPlotItem::Rtti_PlotCurve );
    ui->qwtPlot->detachItems( QwtPlotItem::Rtti_PlotMarker);

    showTime = "The plot was updated on " + QDateTime::currentDateTime().toString(Qt::ISODate);
    ui->label->setText(showTime);
    plotLineGraph(csvmodel);
    haveSaved.setText("Your have saved!");
    haveSaved.exec();
    }
    else{
        QMessageBox noReadMessage;
        noReadMessage.setText("No file is read!");
        noReadMessage.exec();
    }
}



void MS1::on_actionExit_triggered()
{
    useFile=false;
    QApplication::quit();
}


