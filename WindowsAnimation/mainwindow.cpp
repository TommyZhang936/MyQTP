#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPropertyAnimation>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//下坠
void MainWindow::on_pushButton_clicked()
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "geometry");

    QDesktopWidget *pDesktopWidget = QApplication::desktop();
    int x = (pDesktopWidget->availableGeometry().width() - width()) / 2;
    int y = (pDesktopWidget->availableGeometry().height() - height()) / 2;

    pAnimation->setDuration(3000);
    pAnimation->setStartValue(QRect(x, 0, width(), height()));
    pAnimation->setEndValue(QRect(x, y, width(), height()));
    pAnimation->setEasingCurve(QEasingCurve::OutElastic);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
//抖动
void MainWindow::on_pushButton_2_clicked()
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "pos");
    pAnimation->setDuration(500);
    pAnimation->setLoopCount(2);
    pAnimation->setKeyValueAt(0, QPoint(geometry().x() - 3, geometry().y() - 3));
    pAnimation->setKeyValueAt(0.1, QPoint(geometry().x() + 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.2, QPoint(geometry().x() - 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.3, QPoint(geometry().x() + 6, geometry().y() - 6));
    pAnimation->setKeyValueAt(0.4, QPoint(geometry().x() - 6, geometry().y() - 6));
    pAnimation->setKeyValueAt(0.5, QPoint(geometry().x() + 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.6, QPoint(geometry().x() - 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(0.7, QPoint(geometry().x() + 6, geometry().y() - 6));
    pAnimation->setKeyValueAt(0.8, QPoint(geometry().x() - 6, geometry().y() - 6));
    pAnimation->setKeyValueAt(0.9, QPoint(geometry().x() + 6, geometry().y() + 6));
    pAnimation->setKeyValueAt(1, QPoint(geometry().x() - 3, geometry().y() - 3));
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
//透明
void MainWindow::on_pushButton_3_clicked()
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "windowOpacity");
    pAnimation->setDuration(3000);
    pAnimation->setKeyValueAt(0, 1);
    pAnimation->setKeyValueAt(0.5, 0);
    pAnimation->setKeyValueAt(1, 1);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
//飞来
void MainWindow::on_pushButton_4_clicked()
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "geometry");

    QDesktopWidget *pDesktopWidget = QApplication::desktop();
    int x = (pDesktopWidget->availableGeometry().width() - width()) / 2;
    int y = (pDesktopWidget->availableGeometry().height() - height()) / 2;
    int xMax = 400;
    int yMax = 300;

    pAnimation->setDuration(5000);
    pAnimation->setStartValue(QRect(x, y, 0, 0));
    pAnimation->setEndValue(QRect((x - xMax / 2), (y - yMax / 2), xMax, yMax));
    pAnimation->setEasingCurve(QEasingCurve::OutElastic);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
