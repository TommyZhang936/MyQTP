#ifndef HILIGHTBUTTON_H
#define HILIGHTBUTTON_H

#include <QObject>
#include <QWidget>

class HiLightButton : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int radius READ getRadius WRITE setRadius)

public:
    explicit HiLightButton(QWidget *parent = 0);
    int getRadius() {return radius;}
    void setRadius(int intR)
    {
        radius = intR;
        update();
    }

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    int radius;
};

#endif // HILIGHTBUTTON_H
