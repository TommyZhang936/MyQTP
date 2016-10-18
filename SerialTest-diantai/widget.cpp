#include "widget.h"
#include "ui_widget.h"
#include <QCoreApplication>
#include <QImage>
#include <QBuffer>
#include  <math.h>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotSendData()));

    serialPort = new QSerialPort(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serialPort->setPort(info);
        if(serialPort->open(QIODevice::ReadWrite))
        {
            ui->comboBoxCom->addItem(info.portName());
            ui->textEditInfo->append(QStringLiteral("检测到端口列表"));
            ui->textEditInfo->append(info.portName());
            ui->textEditInfo->append(info.description());
            ui->textEditInfo->append(info.manufacturer());
            serialPort->close();
        }
    }

    connect(serialPort,SIGNAL(readyRead()),this,SLOT(slotReadData()));
    ui->comboBoxBaudRate->setCurrentIndex(0);
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonCloseCom->setEnabled(false);
    QString picPath=QCoreApplication::applicationDirPath()+"/1.jpg";
    qDebug() << picPath;
    pixmap.load(picPath);
}

Widget::~Widget()
{
    delete ui;
    if(timer->isActive())
    {
        timer->stop();
    }
}

void Widget::slotSendData()
{
    matrix.rotate(90);
    QPixmap tempPixmap = pixmap.transformed(matrix);
    QBuffer buffer;
    tempPixmap.save(&buffer,"jpg");
    ui->labelImage->setPixmap(tempPixmap);

    char *data=(char*)buffer.data().data();
    int dataLength=buffer.data().length();
    //打印图片大小
    qDebug()<<"Image Size:"<<dataLength;
    int standPacketSize=120;
    int packetSize=0;
    int packetNum=ceil(dataLength/120.0);

    if(dataLength>120)
    {
        for(int i=0;i<packetNum;i++)
        {
            if(standPacketSize*(i+1)<dataLength)
            {
                packetSize=standPacketSize;
            }
            else
            {
                packetSize=dataLength-standPacketSize*i;

            }

            serialPort->write(data,packetSize);
            data=data+packetSize;
        }
    }

    serialPort->write("###",3);
}
void Widget::slotReadData()
{
    QByteArray temp = serialPort->readAll();

    if(!temp.isEmpty())
    {
        byteArray.append(temp);
        if(byteArray.contains("###"))
        {

            QByteArray array=byteArray.left(byteArray.indexOf("###"));
            QImage image;
            bool flag=image.loadFromData((const uchar *)array.data(),array.size());
            if (flag)
            {

                QPixmap pixmap=QPixmap::fromImage(image);
                ui->labelImage->setPixmap(pixmap);

            }
            byteArray = byteArray.right(byteArray.length()-byteArray.indexOf("###")-3);
        }
    }
}

void Widget::on_pushButtonStart_clicked()
{
    timer->start(1000);
}
void Widget::on_pushButtonStop_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
    }
}
void Widget::on_pushButtonOpenCom_clicked()
{
    serialPort->setPortName(ui->comboBoxCom->currentText());
    serialPort->open(QIODevice::ReadWrite);
    serialPort->setBaudRate(ui->comboBoxBaudRate->currentText().toInt());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    ui->pushButtonOpenCom->setEnabled(false);
    ui->pushButtonCloseCom->setEnabled(true);
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonStop->setEnabled(true);

}
void Widget::on_pushButtonCloseCom_clicked()
{
    if(serialPort->isOpen())
    {
        serialPort->close();
        ui->pushButtonOpenCom->setEnabled(true);
        ui->pushButtonCloseCom->setEnabled(false);
        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonStop->setEnabled(false);
    }
}

QString Widget::getCurrentTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return dateTime.toString("hh-mm-ss-zzz");
}


void Widget::delay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}
