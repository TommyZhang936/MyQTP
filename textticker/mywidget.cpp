#include "mywidget.h"
#include "ui_mywidget.h"
#include "textticker.h"
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    m_strTicker ="";
    m_timeTicker = 100;
    QWidget* pParent = (QWidget*)QApplication::desktop();
    text_Ticker = new TextTicker();
    text_Ticker->setTickerText(m_strTicker);
    text_Ticker->startTicker(m_timeTicker);
    int fft = 16;
    text_Ticker->setTickerFont(fft, Qt::white);
    text_Ticker->setRect(QRect(0, 500, pParent->width(),  (int)(1.5 * fft + fft / 3)));


    text_Ticker->show();
}

MyWidget::~MyWidget()
{
    delete ui;
    delete text_Ticker;
}

void MyWidget::on_pushButton_clicked()
{
    m_strTicker = "这是第一个字符串str1234567890~";
    text_Ticker->setBGColor(QColor(200, 200, 0));
    text_Ticker->setTickerText(m_strTicker);
}

void MyWidget::on_pushButton_2_clicked()
{
    m_strTicker = QStringLiteral("!@!&*%*  这是第2个字符串 str  萨法问去速读法，吗 你阿萨德美女，阿萨姆多年来看你喽单色， 玛纳斯，雷德克纳斯，吗弄的我，吗你，美女       1234567890~");
    text_Ticker->setBGColor(QColor(200, 0, 0));
    text_Ticker->setTickerText(m_strTicker);
}

void MyWidget::on_pushButton_up_clicked()
{
    qDebug()<< "Timer : " << m_timeTicker;
    if(m_timeTicker > 0)
    {
        m_timeTicker -=100;
        text_Ticker->startTicker(m_timeTicker);
    }
}

void MyWidget::on_pushButton_down_clicked()
{
    qDebug()<< "Timer : " << m_timeTicker;
    if(m_timeTicker < 5000)
    {
        m_timeTicker +=100;
        text_Ticker->startTicker(m_timeTicker);
    }
}

void MyWidget::on_pushButton_3_clicked()
{
    bool tickerB = text_Ticker->isHidden() ? false : true;
    text_Ticker->setHidden(tickerB);
}
