#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QTime>
#include <QPixmap>
#include <QMatrix>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private:
    QString getCurrentTime();

private:
    Ui::Widget *ui;
    QSerialPort *serialPort;
    QTimer *timer;
    QByteArray byteArray;
    QPixmap pixmap;
    QMatrix matrix;
private:
    void delay(int msec);
private slots:
    void slotSendData();
    void slotReadData();
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonOpenCom_clicked();
    void on_pushButtonCloseCom_clicked();
};

#endif // WIDGET_H
