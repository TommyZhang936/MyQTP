#include "SearchButton.h"
#include <QLineEdit>
#include <QHBoxLayout>

SearchButton::SearchButton(const QString &text, QLineEdit *edit)
    : QPushButton(text, edit) 
{
    setText(text);
    adjustSize();
    int labelWidth = this->geometry().width();
    thisEdit = edit;
    setFixedHeight(thisEdit->sizeHint().height());             //设置按钮高度为LineEdit高度
    setFocusPolicy(Qt::NoFocus);                                   // 得到焦点时，不显示虚线框
    setFlat(true);
    setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addStretch();
    buttonLayout->addWidget(this);    
    thisEdit->setLayout(buttonLayout);

    setPBWidth(labelWidth);

    // 设置默认PB style sheet
    // 为了方便起见, 帮把 style sheet 写到代码里, 实际工作中应该放到专用的style sheet里, 方便修改
    QString qss = QString(".SearchButton {background: gray; color: white; border: 1 solid gray; min-width: 20px; }")
            + QString(".SearchButton:hover {background: rgb(200, 0, 0); color: white; border: 1 solid gray; }")
            + QString(".SearchButton:pressed {background: rgb(0, 200, 100); color: black;}");
    setStyleSheet(qss);
}

void SearchButton::setPBStyle(const QString &stylePB)
{
    setStyleSheet(stylePB);
}

void SearchButton::setPBWidth(const int widthPB)
{
    setFixedWidth(widthPB);
    // 设置输入框中文件输入区，不让输入的文字在被隐藏在按钮下
    thisEdit->setTextMargins(0, 1, widthPB, 1);
}
