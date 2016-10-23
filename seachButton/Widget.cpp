#include "Widget.h"
#include "ui_Widget.h"
#include "SearchButton.h"
#include "Textlineedit.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    SearchButton *newSPB1 =  new SearchButton(tr("搜索元器件"), ui->lineEdit_1);
    newSPB1->setPBWidth(100);
    SearchButton *newSPB2 = new SearchButton(tr("搜索"), ui->lineEdit_2);
    newSPB2->setPBWidth(80);
    TextLineEdit *newLineEdit = new TextLineEdit("编辑出生年月：");
    //ui->lineEdit = newLineEdit;
    this->layout()->addWidget(newLineEdit);
//    QString str = QString("border-width: 1px; border-style: solid; border-color: rgb(255, 170, 0);");
//    newLineEdit->setLabelStyle(str);
//    newLineEdit->setLabelWidth(100);
}

Widget::~Widget() {
    delete ui;
}
