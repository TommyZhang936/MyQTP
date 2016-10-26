#ifndef CHINESENUMBER_H
#define CHINESENUMBER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class ChineseNumber : public QWidget
{
    Q_OBJECT
public:
    explicit ChineseNumber(QWidget *parent = 0);
    ~ChineseNumber();
    void setStart(const int &intValue);

signals:

public slots:

private:
    void showHZ();
    QString setHZ(const int &);

    int m_number;
    int m_red;
    int m_green;
    int m_blue;
    const int m_plus[3][6] ={{1, 0, 0, -1, 0, 0}, {0, 1, 0, 0, -1, 0}, {0, 0, 1, 0, 0, -1}};
    int m_rgbindex;
    QStringList cNumber;

    QLabel *m_label;
    QTimer *timerTicker;
};

#endif // CHINESENUMBER_H
