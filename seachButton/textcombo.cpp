#include "TextCombo.h"
#include <QComboBox>
#include <QLabel>
#include <QDebug>

TextCombo::TextCombo(const QString &text)
    : QComboBox()
{
    //定制Label
    textLabel = new QLabel(this);
    textLabel->setText(text);
    textLabel->adjustSize();
    textLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    textLabel->setFocusPolicy(Qt::NoFocus);                 // 得到焦点时，不显示虚线框
    int labelWidth = textLabel->geometry().width();       //得到Label的宽度
    textLabel->setFixedHeight(this->sizeHint().height());                                  // 设置按钮的大小为图片的大小
    //设置Label宽度和ComboBox文字显示范围
    setLabelWidth(labelWidth);

    // 设置Label默认样式    
    QString str = QString("background: black; color: white; border: 1 solid gray; ");
    setLabelStyle(str);            
}

TextCombo::~TextCombo()
{
    delete textLabel;
}

void TextCombo::setLabelStyle(const QString &styleLabel)
{
    textLabel->setStyleSheet(styleLabel);
}

void TextCombo::setLabelWidth(const int widthLabel)
{
    textLabel->setFixedWidth(widthLabel);
    //设置输入框中文件输入区，不让输入的文字在被隐藏在Label文本下
    QString str;// = QString(".TextCombo { padding: 0px 0px 0px %1px; }").arg(widthLabel);
    str = QString(" QComboBox QAbstractItemView::item { min-height:40px; min-width: 100px; } ");
    qDebug() << str;
    //setContentsMargins(100, 0, 0, 0);
    this->setStyleSheet(str);
}
