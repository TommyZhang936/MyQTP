#include "Widget.h"
#include "ui_Widget.h"
#include "SearchButton.h"
#include "Textlineedit.h"
#include "textcombo.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    SearchButton *newSPB1 =  new SearchButton(tr("搜索元器件"), ui->lineEdit_1);
    newSPB1->setPBWidth(100);
    SearchButton *newSPB2 = new SearchButton(tr("搜索"), ui->lineEdit_2);
    newSPB2->setPBWidth(80);
    TextLineEdit *newLineEdit = new TextLineEdit("编辑出生年月：");
    this->layout()->addWidget(newLineEdit);
//    QString str = QString("border-width: 1px; border-style: solid; border-color: rgb(255, 170, 0);");
//    newLineEdit->setLabelStyle(str);
//    newLineEdit->setLabelWidth(100);
    TextCombo *newTextCombo = new TextCombo("选择性别：");
    this->layout()->addWidget(newTextCombo);
    //newTextCombo->setLabelWidth(80);
    newTextCombo->addItem("男asd 打死阿萨德阿萨德阿萨德");
    newTextCombo->addItem("女");


    
}

Widget::~Widget() {
    delete ui;
}
