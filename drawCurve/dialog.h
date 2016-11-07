#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QTimer>
#include<QPainterPath>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QPoint Point;   /* �������Ƶ������ */

    void startPainting(void);    /* �����滭���� Ҳ���ǿ�����ʱ�� */
    void closePainting(void);    /* ֹͣ�滭���� Ҳ����ֹͣ��ʱ�� */


    void paintEvent(QPaintEvent *); /* �ػ��¼����� */

private slots:
    void timerUpDate();     /* ��ʱ���ۺ��� */

private:
    Ui::Dialog *ui;

    unsigned int t; /* ��������ʱ�� */
    int p; /* ������������ԭ�� */

    QPainterPath *path;
    QTimer *timer;
};

#endif // DIALOG_H
