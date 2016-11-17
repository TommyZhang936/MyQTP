#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include "particlemachine.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void timeOut();
    void emitSome();

private:
    void debug_msg(QString msg);

private:
    ParticleMachine *m_machine;
    QTimer *m_timer;
    QTimer *m_emiter;
    QPoint m_mousePos;
    int m_updateSpeed;

    QStringList m_messages;
};

#endif // MAINWINDOW_H
