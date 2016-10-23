#include "inputlineedit.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>

InputLineEdit::InputLineEdit(const QString &text, QLineEdit *edit)
    : QLabel(text, edit) 
{
    setText(text);
    adjustSize();
    int labelWidth = geometry().width();
    setFocusPolicy(Qt::NoFocus);    // 得到焦点时，不显示虚线框
    
    QSize size = QSize(labelWidth, edit->sizeHint().height());
    //setBaseSize(size);
    setFixedSize(size);
//    setMinimumSize(size);
//    setMaximumSize(size);           // 设置按钮的大小为图片的大小
    

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(this);
    buttonLayout->addStretch(1);
    
    edit->setLayout(buttonLayout);

    // 设置输入框中文件输入区，不让输入的文字在被隐藏在按钮下
    edit->setTextMargins(labelWidth, 1, 0, 1);

    // 设置style sheet
    // 为了方便起见, 帮把 style sheet 写到代码里, 实际工作中应该放到专用的style sheet里, 方便修改
    QString qss = QString(".InputLineEdit {background: gray; color: white; border: 1 solid gray;min-width: 40px;}");
//            + QString(".InputLineEdit:hover {background: black; color: white; border: 1 solid black;}")
//            + QString(".InputLineEdit:pressed {background: white;color: black;}");
    setStyleSheet(qss);
}
