/*
* Copyright (c) 2016，鼎深普技术有限公司
* All rights reserved.
*
* 摘要：简要描述本文件的内容
* 修改时间：20160927
* 修改内容：
* 1、增加数据库表对QPushButton和QComboBox控件的支持
* 2、修改字段Layout和命令按钮Layout布局的方式
* 3、增加控件类型错误处理（但是QPushButton处理还不完美）
* 4、增加读取SQLite数据库表字段名称和类型的方法
* 修改时间：20160928
* 修改内容：
* 1、将表设计和实现修改为“文字标签“和“数据控件“一体存储
* 2、读取数据库表结构
* 3、将读取到的记录值和字段一一对应，取值将不受SQL顺序、字段增减影响
*       可以使用SELECT *读取字段
* 修改时间：20161014
* 修改内容：
* 1、增加ComboBox栏目处理，用字段strDispString作comboBox的列表
*       1）首字符1表示直接取后面的信息，用‘，’分隔项目
*       2）首字符2表示从数据库取数据，用WidgetName和strCtrlObjName从combolist表取数
*/

#include "mainwindow.h"
#include "ctrlshow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //调用CtrlShow创建新窗口
    CtrlShow *newCtrlShow = new CtrlShow(this);
    setCentralWidget(newCtrlShow);    
    newCtrlShow->ctrlShowInit("Widget001");
    
    QFont ft;
    ft.setFamily("微软雅黑");
    newCtrlShow->setFont(ft);

    resize(400, 150);

    QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow()
{
}


void MainWindow::on_pBOK_clicked()
{
    qDebug() << "PushButton clicked() OK!";
}

void MainWindow::on_pBCancel_clicked()
{
    qDebug() << "PushButton clicked() Cancel!";
}
