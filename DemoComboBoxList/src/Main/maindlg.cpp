#include "maindlg.h"
#include "ui_maindlg.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QPainter>

MainDlg::MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDlg)
{
    ui->setupUi(this);
    Init();
}

MainDlg::~MainDlg()
{
    delete ui;
}

void MainDlg::Init()
{
    colorCombo = ui->comboBoxColor;
    fillColorList(colorCombo);
    connect(colorCombo, SIGNAL(activated(int)), this, SLOT(showColor()));
    dbCombo = ui->comboBoxDB;
    fillDBList(dbCombo);
    connect(dbCombo, SIGNAL(activated(int)), this, SLOT(showDB()));
}

void MainDlg::fillColorList(QComboBox *cbb)
{
    /*
    QStringList colorNameList = QColor::colorNames();  //读取所有预定义颜色名字
    //cbb->addItems(colorNameList);

    QString colorName;
    foreach(colorName, colorNameList)
    {
        //qDebug() << colorName;
        QPixmap pix_color(20, 20);
        pix_color.fill(QColor(colorName));

        cbb->addItem(QIcon(pix_color), colorName);
        cbb->setIconSize(QSize(20, 20));
        cbb->setSizeAdjustPolicy(QComboBox::AdjustToContents);   //设置下拉列表的尺寸符合内容的大小
    }

    */
 QComboBox *comBox;
 comBox = cbb;;
 QPixmap pix(16,16); //创建绘图设备
 QPainter painter(&pix);//创建一个画笔
 //黑色
 painter.fillRect(0,0,16,16,Qt::black);
 comBox->addItem(QIcon(pix),tr("黑色"),QColor(Qt::black));
 //红色
 painter.fillRect(0,0,16,16,Qt::red);
 comBox->addItem(QIcon(pix),tr("红色"),QColor(Qt::red));
 //green
 painter.fillRect(0,0,16,16,Qt::green);
 comBox->addItem(QIcon(pix),tr("绿色"),QColor(Qt::green));
 //blue
 painter.fillRect(0,0,16,16,Qt::blue);
 comBox->addItem(QIcon(pix),tr("蓝色"),QColor(Qt::blue));
//yellow
 painter.fillRect(0,0,16,16,Qt::yellow);
 comBox->addItem(QIcon(pix),tr("黄色"),QColor(Qt::yellow));
 //cyan
 painter.fillRect(0,0,16,16,Qt::cyan);
 comBox->addItem(QIcon(pix),tr("蓝绿色"),QColor(Qt::cyan));
 //洋红
 painter.fillRect(0,0,16,16,Qt::magenta);
 comBox->addItem(QIcon(pix),tr("洋红"),QColor(Qt::magenta));

 //下面是关于上面语句中直接使用 Qt::red 编译出错的解释
 // These constructors don't create QVariants of the type associcated with the enum, as expected,
 // but they would create a QVariant of type int with the value of the enum value.
 // Use QVariant v = QColor(Qt::red) instead of QVariant v = Qt::red for example.
 // Because QVariant is part of the QtCore library,
 // it cannot provide conversion functions to data types defined in QtGui,
 // such as QColor, QImage, and QPixmap. In other words, there is no toColor() function.
 // Instead, you can use the QVariant::value() or the qvariant_cast() template function.

}

void MainDlg::fillDBList(QComboBox *cbb)
{
    if(connectSQLiteDB("ComboBox.db"))
    {
        QSqlQuery sql_query;
        //读取字段，可根据需要读取不同的字段
        QString select_sql = "SELECT ComboBoxName, RealName FROM systype WHERE Type = 'DBCOMBOBOX' ORDER BY ComboBoxName";

        //sql_query.prepare(select_sql);
        if(!sql_query.exec(select_sql))
        {
            qDebug() << sql_query.lastError();
        }
        else
        {
            //构造MAP数据为后面使用
            dbNameMap.clear();

            while(sql_query.next())
            {
                //将读取的字段用于构造MAP，以便后面使用
                QString tNameB = sql_query.value(0).toString();
                QString tNameR = sql_query.value(1).toString();
                dbNameMap.insert(tNameB, tNameR);

                QPixmap pix_color(20, 20);
                pix_color.fill(QColor(rand() % 256, rand() % 256, rand() % 256));
                cbb->addItem(pix_color, tNameB);
                cbb->setIconSize(QSize(20, 20));
                //cbb->setSizeAdjustPolicy(QComboBox::AdjustToContents);   //设置下拉列表的尺寸符合内容的大小
            }
            //qDebug() << dbNameMap << endl << dbNameMap.values();

        }
    }
}

void MainDlg::showColor()
{
    QStringList colorNameList = QColor::colorNames();
    QColor m_color = QColor(colorNameList[colorCombo->currentIndex()]);
    qDebug() << m_color;
}

void MainDlg::showDB()
{
    QString m_DB = dbNameMap.value(dbCombo->currentText());
            //dbCombo->currentText();
    qDebug() << m_DB;
    for (int i = 0; i < 14 ; i++)
    {
        QVariant m_Selected = dbCombo->currentData(i);
        qDebug() << i << " : " << m_Selected;
    }
}

bool MainDlg::connectSQLiteDB(const QString nameDB)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(nameDB);
    //database.setUserName("root");
    //database.setPassword("123456");

    //打开数据库
    if(!database.open())
    {
        qDebug() << database.lastError();
        qFatal("failed to connect!") ;
        return false;
    }
    return true;
}
