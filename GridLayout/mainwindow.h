/*
* Copyright (c) 2016，鼎深普技术有限公司
* All rights reserved.
*
* 文件名称：mainwindow.h
* 文件标识：
* 摘要：
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class CtrlShow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pBOK_clicked();
    void on_pBCancel_clicked();


private:
    QWidget *widgetCentral;
    CtrlShow *newCtrlShow;

};

#endif // MAINWINDOW_H
