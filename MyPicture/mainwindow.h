#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color);

public slots:    
    void updatePaint();
    
protected:
    void paintEvent(QPaintEvent *);
    
private:
    Ui::MainWindow *ui;
    
    int m_nRotationAngle;
};

#endif // MAINWINDOW_H
