#include "TextLineEdit.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>

TextLineEdit::TextLineEdit(const QString &text)
    : QLineEdit()
{
    //定制Label
    textLabel = new QLabel(this);
    textLabel->setText(text);
    textLabel->adjustSize();
    textLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    textLabel->setFocusPolicy(Qt::NoFocus);                 // 得到焦点时，不显示虚线框
    int labelWidth = textLabel->geometry().width();       //得到Label的宽度
    //QSize size = QSize(labelWidth, this->sizeHint().height());
    textLabel->setFixedHeight(this->sizeHint().height());                                  // 设置按钮的大小为图片的大小
    //设置Label宽度和LineEdit可用范围
    setLabelWidth(labelWidth);

    // 设置Label默认样式
    QString str = QString("background: black; color: white; border: 1 solid gray;");
    setLabelStyle(str);
}

TextLineEdit::~TextLineEdit()
{
    delete textLabel;
}

void TextLineEdit::setLabelStyle(const QString &styleLabel)
{
    textLabel->setStyleSheet(styleLabel);
}

void TextLineEdit::setLabelWidth(const int widthLabel)
{
    textLabel->setFixedWidth(widthLabel);
    // 设置输入框中文件输入区，不让输入的文字在被隐藏在按钮下
    setTextMargins(widthLabel, 1, 0, 1);
}
