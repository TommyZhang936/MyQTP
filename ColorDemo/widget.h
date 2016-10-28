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
    void paintEvent(QPaintEvent *event);

private:
    Ui::Widget *ui;

    void ReadColorTXT();
    void showColor();

    QList<QString> listColor;
    QHash<QString, QString> hashRGB;
    
    int numColor;
    int numRed;
    int numGreen;
    int numBlue;
    int numWidth;
    int numHeight;
    int pxWidth;
    int pxHeight;
    int sppxWidth;
    int sppxHeight;
    int startWidth;
    int startHeight;
    
};

#endif // WIDGET_H
