#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QVector>
#include <QWidget>
#include <QString>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QIODevice>
#include <QValueAxis>

#include "chartview.h"
//#include "callout.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    //界面初始化显示设置
   // void WidgetInitial();

    void updateSerialPortNumber();
    void updateReceive();
    void setBaudRate();
    //void tipSlot(QPointF position, bool isHovering);

   // void serialPort_readyRead();

    void on_openButton_clicked();
    void on_closeButton_clicked();
    void on_beginButton_clicked();
    void on_pauseButton_clicked();
    void on_clearButton_clicked();
    void on_dataFileButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    ChartView *Freq_chartView;
    ChartView *Phase_chartView;
    ChartView *Inc_chartView;
    QChart *Freq_chart = new QChart();
    QChart *Phase_chart = new QChart();
    QChart *Inc_chart = new QChart();
    //Callout *tip;
    QSplineSeries *Xe129_series;
    //QSplineSeries *Xe131_series;
    QSplineSeries *Phase_series;
    QSplineSeries *Inc_series;
    QString dataFilePath;
    //QFile dataFile;

    QTimer *timer_receiveData = new QTimer();
    void initChart();
    void wheelEvent(QWheelEvent *event);
    void delay(int msec);
    //QFile *createFile(QString filePath, QString fileName);
};

#endif // MAINWINDOW_H
