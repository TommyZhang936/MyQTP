#include "Widget.h"
#include "ui_Widget.h"
#include "SearchButton.h"
#include "inputlineedit.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    new SearchButton(tr("搜索元器件"), ui->lineEdit_1);
    new SearchButton(tr("搜索"), ui->lineEdit_2);
    new InputLineEdit("编辑出生年月：", ui->lineEdit_3);
}

Widget::~Widget() {
    delete ui;
}
