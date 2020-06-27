#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateSerialPortNumber();
    initChart();

    timer_receiveData->start(100);

    //connect(series, SIGNAL(hovered(QPointF,bool)), this, SLOT(tipSlot(QPointF,bool)));
    //connect(series, &QSplineSeries::hovered, this, &MainWindow::tipSlot);

    // 按键失能
    ui->beginButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->closeButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        Freq_chart->zoom(1.1);
        Phase_chart->zoom(1.1);
        Inc_chart->zoom(1.1);
    } else {
        Freq_chart->zoom(10.0/11);
        Phase_chart->zoom(10.0/11);
        Inc_chart->zoom(10.0/11);
    }

    QWidget::wheelEvent(event);
}

void MainWindow::initChart()
{
    /********************************Ferq View******************************/
    Xe129_series = new QSplineSeries();
    Xe129_series->setName("Xe129");
    Xe129_series->setColor(Qt::red);

    Freq_chart->addSeries(Xe129_series);

    QValueAxis *Freq_axisX = new QValueAxis;
    QValueAxis *Freq_axisY = new QValueAxis;

    Freq_axisY->setRange(17.8, 18.2);
    Freq_axisX->setRange(0, 299);

    Freq_axisX->setLabelFormat("%d");   //设置刻度的格式
    Freq_axisY->setLabelFormat("%2.2f");
    Freq_axisX->setGridLineVisible(true); //网格线可见
    Freq_axisY->setGridLineVisible(true);
    Freq_axisX->setTickCount(30);       //设置多少格
    Freq_axisY->setTickCount(11);
    Freq_axisX->setMinorTickCount(1);   //设置每格小刻度线的数目
    Freq_axisY->setMinorTickCount(1);

    Freq_axisX->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    Freq_axisY->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    Freq_axisX->setTitleText("时间/s");
    Freq_axisY->setTitleText("频率/Hz");

    Freq_chart->legend()->hide();
    Freq_chart->setAxisX(Freq_axisX, Xe129_series);       //为chart设置X轴,并把轴附加到series线上
    Freq_chart->setAxisY(Freq_axisY, Xe129_series);
    /***********************************************************************/

    /********************************Xe131 View******************************/
//    Xe131_series = new QSplineSeries();
//    Xe131_series->setName("Xe131");
//    Xe131_series->setColor(Qt::blue);

//    Freq_chart->addSeries(Xe131_series);

//    Freq_chart->setAxisX(Freq_axisX, Xe131_series);
//    Freq_chart->setAxisY(Freq_axisY, Xe131_series);

    Freq_chartView = new ChartView(Freq_chart);
    Freq_chartView->setRenderHint(QPainter::Antialiasing);
    ui->Ferq_canvas->addWidget(Freq_chartView);
    /************************************************************************/

    /********************************Phase View******************************/
    Phase_series = new QSplineSeries();
    Phase_series->setName("Phase");
    //Phase_series->setColor(Qt::red);

    Phase_chart->addSeries(Phase_series);

    QValueAxis *Phase_axisX = new QValueAxis;
    QValueAxis *Phase_axisY = new QValueAxis;

    Phase_axisY->setRange(-1.5, 1.5);
    Phase_axisX->setRange(0, 299);

    Phase_axisX->setLabelFormat("%d");   //设置刻度的格式
    Phase_axisY->setLabelFormat("%2.1f");
    Phase_axisX->setGridLineVisible(true); //网格线可见
    Phase_axisY->setGridLineVisible(true);
    Phase_axisX->setTickCount(30);       //设置多少格
    Phase_axisY->setTickCount(11);
    Phase_axisX->setMinorTickCount(1);   //设置每格小刻度线的数目
    Phase_axisY->setMinorTickCount(1);

    Phase_axisX->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    Phase_axisY->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    Phase_axisX->setTitleText("时间/s");
    Phase_axisY->setTitleText("相位/°");

    Phase_chart->legend()->hide();
    Phase_chart->setAxisX(Phase_axisX, Phase_series);       //为chart设置X轴,并把轴附加到series线上
    Phase_chart->setAxisY(Phase_axisY, Phase_series);
    Phase_chartView = new ChartView(Phase_chart);
    Phase_chartView->setRenderHint(QPainter::Antialiasing);
    ui->Phase_canvas->addWidget(Phase_chartView);
    /***********************************************************************/

    /********************************Inc View******************************/
    Inc_series = new QSplineSeries();
    Inc_series->setName("Increment");
    //Phase_series->setColor(Qt::red);

    Inc_chart->addSeries(Inc_series);

    QValueAxis *Inc_axisX = new QValueAxis;
    QValueAxis *Inc_axisY = new QValueAxis;

    Inc_axisY->setRange(-1, 1);
    Inc_axisX->setRange(0, 299);

    Inc_axisX->setLabelFormat("%d");   //设置刻度的格式
    Inc_axisY->setLabelFormat("%2.1f");
    Inc_axisX->setGridLineVisible(true); //网格线可见
    Inc_axisY->setGridLineVisible(true);
    Inc_axisX->setTickCount(30);       //设置多少格
    Inc_axisY->setTickCount(11);
    Inc_axisX->setMinorTickCount(1);   //设置每格小刻度线的数目
    Inc_axisY->setMinorTickCount(1);

    Inc_axisX->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    Inc_axisY->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    Inc_axisX->setTitleText("时间/s");
    Inc_axisY->setTitleText("增量");

    Inc_chart->legend()->hide();
    Inc_chart->setAxisX(Inc_axisX, Inc_series);       //为chart设置X轴,并把轴附加到series线上
    Inc_chart->setAxisY(Inc_axisY, Inc_series);
    Inc_chartView = new ChartView(Inc_chart);
    Inc_chartView->setRenderHint(QPainter::Antialiasing);
    ui->Inc_canvas->addWidget(Inc_chartView);
    /***********************************************************************/

}

void MainWindow::updateReceive()
{
    int i, j, k;
    QFile dataFile(dataFilePath);
    if(!dataFile.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        //qDebug()<<QObject::tr("文件打开失败");
    }
    QTextStream out(&dataFile);

    qint64 Xe129_size = Xe129_series->pointsVector().size();
    //qint64 Xe131_size = Xe131_series->pointsVector().size();
    qint64 Phase_size = Phase_series->pointsVector().size();
    qint64 Inc_size = Xe129_series->pointsVector().size();



    QString content = ui->pauseButton->text();
    QString stopSignal = ui->beginButton->text();

    // 从接收缓冲区中读取数据
    QByteArray buffer;
    buffer.clear();
    buffer = serial.readAll();
    //qDebug() << "Buffer Size:" << buffer.size() << '\n';

    if ((buffer.size() == 28) || (buffer.size() == 29) || (buffer.size() == 30) || (buffer.size() == 31) || (buffer.size() == 32))
    {
        //qDebug() << "data:" << buffer << '\n';

        QList<QByteArray> dataRec1, dataRec2;
        dataRec1.clear();
        dataRec2.clear();
        dataRec1 = buffer.split('\n');

        int dataSize = 0;
        dataSize = dataRec1.size();
        //qDebug() << "data1 size:" << dataSize << '\n';

        float *Xe129, /* *Xe131 */ *Phase, *Inc;
        Xe129 = new float[dataSize];
        //Xe131 = new float[dataSize];
        Phase = new float[dataSize];
        Inc = new float[dataSize];

        for(i = 0; i < dataSize - 1; i++)
        {
            //qDebug() << "******* dataRec1 *******:" << dataRec1[i] << '\n';
            dataRec2.append(dataRec1[i].split(' '));
        }
        //qDebug() << "data2 size:" << dataRec2.size() << '\n';

        for(j = 0; j < dataRec2.size(); j++)
        {
            //qDebug() << "####### dataRec2 #######:" << dataRec2[j] << '\n';
//            if((j % 2) == 0 && (j % 4) !=0){
//                Xe131[j/2] = dataRec2[j].toFloat();
//            }else
            switch (j) {
            case 0:
                Xe129[j] = dataRec2[j].toFloat();
                break;
            case 1:
                Phase[0] = dataRec2[j].toFloat();
                break;
            case 2:
                Inc[0] = dataRec2[j].toFloat();
                break;
            default:
                break;
            }
//            if(j == 0){
//                Xe129[j] = dataRec2[j].toFloat();
//            }else if(j == 1 ){
//                Phase[0] = dataRec2[j].toFloat();
//            }else if(j == 2){
//                Inc[0] = dataRec2[j].toFloat();
//            }
        }

        //qDebug() << "dataSize:" << dataSize << '\n';

        QList<QPointF> Xe129_points;
        //QList<QPointF> Xe131_points;
        QList<QPointF> Phase_points;
        QList<QPointF> Inc_points;

        if(content.operator ==("暂停采集")){
            if(buffer.isNull()){
            }else{
                    for(k = 0; k < dataSize - 1; k++){
                        QPointF Xe129_point((float)(k + Xe129_size), Xe129[k]);
                        //qDebug() << "------- Xe129 -------:" << Xe129[k] << '\n';
                        //QPointF Xe131_point((float)(k + Xe131_size), Xe131[k]);
                        //qDebug() << "------- Theta -------:" << Theta[k] << '\n';
                        QPointF Phase_point((float)(k + Phase_size), Phase[k]);
                        //qDebug() << "------- Phase -------:" << Phase[k] << '\n';
                        QPointF Inc_point((float)(k + Inc_size), Inc[k]);
                        //qDebug() << "------- Increment -------:" << Inc[k] << '\n';
                        out << Xe129[k] << "     " /*<< Xe131[k] << "     " */<< Phase[k] << "     " << Inc[k] << "\n";
                        Xe129_points.append(Xe129_point);
                        //Xe131_points.append(Xe131_point);
                        Phase_points.append(Phase_point);
                        Inc_points.append(Inc_point);
                    }
                    Xe129_series->append(Xe129_points);
                    //Xe131_series->append(Xe131_points);
                    Phase_series->append(Phase_points);
                    Inc_series->append(Inc_points);
            }
        }else if(content.operator ==("恢复采集")){

            }
     }

    if((Xe129_size >= 300) /*& (content.operator ==("暂停采集")) & (stopSignal.operator == ("停止采集"))*/)
    {
        delay(70);
        Freq_chart->scroll(2, 0);
        Phase_chart->scroll(2, 0);
        Inc_chart->scroll(2, 0);
    }
}

void MainWindow::updateSerialPortNumber()
{
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        ui->portNameBox->clear();
        ui->portNameBox->addItem("暂无可用串口");
        return;
    }
   foreach (QSerialPortInfo info, infos) {
       ui->portNameBox->addItem(info.portName());
   }
}

void MainWindow::on_openButton_clicked()
{
    // 如果串口已经打开了，先关闭
    if(serial.isOpen())
    {
        serial.clear();
        serial.close();
    }

    serial.setPortName(ui->portNameBox->currentText());

    // 打开串口
    bool isOpen = serial.open(QIODevice::ReadWrite);
    if(!isOpen)
    {
        QMessageBox::about(NULL, "提示", "无法打开串口！");
        return;
    }

    QPalette red;
    red.setColor(QPalette::WindowText, Qt::red);
    ui->portState->setPalette(red);
    ui->portState->setText("串口已打开！");

    setBaudRate();
    serial.setDataBits(QSerialPort::Data8);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);

    // 按键失能
    ui->portNameBox->setEnabled(false);
    ui->openButton->setEnabled(false);

    // 按键使能
    ui->beginButton->setEnabled(true);
    ui->pauseButton->setEnabled(true);
    ui->closeButton->setEnabled(true);

    //QObject::connect(&serial, &QSerialPort::readyRead, this, &MainWindow::updateReceive);
    connect(timer_receiveData, SIGNAL(timeout()), this, SLOT(updateReceive()));

}

void MainWindow::on_closeButton_clicked()
{
    // 关闭串口
    serial.close();

    QPalette red;
    red.setColor(QPalette::WindowText, Qt::red);
    ui->portState->setPalette(red);
    ui->portState->setText("串口已关闭！");

    // 按键使能
    ui->portNameBox->setEnabled(true);
    ui->openButton->setEnabled(true);

    // 按键失能
    ui->beginButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->closeButton->setEnabled(false);
    disconnect(timer_receiveData, SIGNAL(timeout()), this, SLOT(updateReceive()));
}

void MainWindow::on_beginButton_clicked()
{
    QString content = ui->beginButton->text();
    if(content.operator ==("开始采集"))
    {
        ui->beginButton->setText("停止采集");
    }else if(content.operator ==("停止采集")){
        ui->beginButton->setText("开始采集");
    }

    //serial.write("1");
}

void MainWindow::on_clearButton_clicked()
{
    //ui->receive->clear();
    Xe129_series->clear();
    //Xe131_series->clear();
    Phase_series->clear();
    Inc_series->clear();
}

void MainWindow::on_pauseButton_clicked()
{
    QString title = ui->pauseButton->text();

    if(title.operator ==("暂停采集"))
    {
        ui->pauseButton->setText("恢复采集");
    }else if(title.operator ==("恢复采集")){
        ui->pauseButton->setText("暂停采集");
    }
}

//设置波特率
void MainWindow::setBaudRate()
{
    int index = ui->baudRateBox->currentIndex();

    switch (index) {
    case 0:
        serial.setBaudRate(QSerialPort::Baud1200);
        break;
    case 1:
        serial.setBaudRate(QSerialPort::Baud2400);
        break;
    case 2:
        serial.setBaudRate(QSerialPort::Baud4800);
        break;
    case 3:
        serial.setBaudRate(QSerialPort::Baud9600);
        break;
    case 4:
        serial.setBaudRate(QSerialPort::Baud19200);
        break;
    case 5:
        serial.setBaudRate(QSerialPort::Baud38400);
        break;
    case 6:
        serial.setBaudRate(QSerialPort::Baud57600);
        break;
    case 7:
        serial.setBaudRate(QSerialPort::Baud115200);
        break;
    case 8:
        serial.setBaudRate(QSerialPort::UnknownBaud);
        break;
    }
}

void MainWindow::on_dataFileButton_clicked()
{
    dataFilePath = QFileDialog::getExistingDirectory(this, "请选择数据保存路径...", "./");

    if(dataFilePath.isEmpty())
    {
        return;
    }else{
        //qDebug() << dataFilePath << endl;
        ui->dataFilePath->setText(dataFilePath);
        dataFilePath.append("/dataFile.txt");
    }
}

void MainWindow::delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 2000);
}
//QFile *MainWindow::createFile(QString filePath,QString fileName)
//{
//    QDir tempDir;
//    //临时保存程序当前路径
//    QString currentDir = tempDir.currentPath();
//    //如果filePath路径不存在，创建它
//    if(!tempDir.exists(filePath))
//    {
//        qDebug()<<QObject::tr("不存在该路径")<<endl;
//        tempDir.mkpath(filePath);
//    }
//    QFile *tempFile = new QFile;
//    //将程序的执行路径设置到filePath下
//    tempDir.setCurrent(filePath);
//    qDebug()<<tempDir.currentPath();
//    //检查filePath路径下是否存在文件fileName,如果存在则停止操作。
//    if(tempFile->exists(fileName))
//    {
//        qDebug()<<QObject::tr("文件存在");
//        //QFile *file = new QFile;
//        //file(filePath.append("/dataFile.txt"));
//        return tempFile;
//    }else{
//        //此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
//        tempFile->setFileName(fileName);
//        if(!tempFile->open(QIODevice::ReadWrite|QIODevice::Text))
//        {
//            qDebug()<<QObject::tr("打开失败");
//        }
//        tempFile->close();
//        //将程序当前路径设置为原来的路径
//        tempDir.setCurrent(currentDir);
//        qDebug()<<tempDir.currentPath();

//        return tempFile;
//    }
//}
//void MainWindow::tipSlot(QPointF position, bool isHovering)
//{
//    if (tip == 0)
//        tip = new Callout(chart);

//    if (isHovering) {
//        tip->setText(QString("X: %1 \nY: %2 ").arg(position.x()).arg(position.y()));
//        tip->setAnchor(position);
//        tip->setZValue(11);
//        tip->updateGeometry();
//        tip->show();
//    } else {
//        tip->hide();
//    }
//}
