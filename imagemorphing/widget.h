#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    virtual void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();
    void imageMorph();

private:


private:
    Ui::Widget *ui;

    int imageLenth;
    bool morphing;

    QTimer *timer;

    QImage picFrom;
    QImage picTo;
    QImage picGoto;
    QRgb rgbFrom[256][256];
    QRgb rgbTo[256][256];
    QRgb rgbGoto[256][256];
//    int redFrom[256][256];
//    int greenFrom[256][256];
//    int blueFrom[256][256];
//    int redTo[256][256];
//    int greenTo[256][256];
//    int blueTo[256][256];

};

#endif // WIDGET_H
