#include "switchbutton.h"
#include <QPushButton>

/* 说明:自定义开关按钮控件实现文件
 * 功能:用来控制配置文件的开关设置
 */
SwitchButton::SwitchButton(QWidget *parent) 
    :QPushButton(parent)
{
    setCursor(QCursor(Qt::PointingHandCursor));
    isCheck = false;
    styleOn = "background-image: url(:/btn_check_on.png); border: 0px;";
    styleOff = "background-image: url(:/btn_check_off.png); border: 0px;";

    setFocusPolicy(Qt::NoFocus);
    setFixedSize(87, 28);                //不允许变化大小
    setStyleSheet(styleOff);           //设置当前样式
    
    connect(this, SIGNAL(clicked()), this, SLOT(ChangeOnOff()));
}

SwitchButton::~SwitchButton()
{
    
}

void SwitchButton::ChangeOnOff()
{
    if (isCheck) 
    {
        setStyleSheet(styleOff);
        isCheck = false;
    } 
    else 
    {
        setStyleSheet(styleOn);
        isCheck = true;
    }
}

//设置当前选中状态
void SwitchButton::SetON(bool isCheck)
{
    if (this->isCheck != isCheck) 
    {
        this->isCheck = !isCheck;
        ChangeOnOff();
    }
}
