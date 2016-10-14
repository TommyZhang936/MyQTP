#include "ctrlshow.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QFontComboBox>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QDateTimeEdit>
#include <QDial>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPlainTextEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMap>
#include <QDebug>

//WidgetName,iDispNo, strUIType, strCtrlType, strLabelString, iDRowLabel, iDColumnLabel, iSpanRowLabel, iSpanColumnLabel, strCtrlObjName, strDispString, iDRow, iDColumn, iSpanRow, iSpanColumn
struct DisplayData
{
    QString WidgetName;
    int iDispNo;
    QString strUIType;
    QString strLabelString;
    int iDRowLabel;
    int iDColumnLabel;
    int iSpanRowLabel;
    int iSpanColumnLabel;
    QString strCtrlType;
    QString strCtrlObjName;
    QString strDispString;
    int iDRow;
    int iDColumn;
    int iSpanRow;
    int iSpanColumn;

};

enum CtrlType
{
    enumCTQComboBox =1,
    enumCTQFontComboBox,
    enumCTQLineEdit,
    enumCTQTextEdit,
    enumCTQPlainTextEdit,
    enumCTQPushButton,
    enumCTQLabel,
    enumCTQDial,
    enumCTQDateEdit,
    enumCTQTimeEdit,
    enumCTQDateTimeEdit,
    enumCTQSpinBox,
    enumCTQDoubleSpinBox,
    Other
};

CtrlShow::CtrlShow(QWidget *parent)
    : QWidget(parent)
{
    if(connectSQLiteDB("DBSystemData"))
    {
        //取表uiinfo字段
        getSQLiteField("uiinfo");

        //初始化控件类型对照Map
        mapCtrlType.clear();
        mapCtrlType.insert("QComboBox", enumCTQComboBox);
        mapCtrlType.insert("QFontComboBox", enumCTQFontComboBox);
        mapCtrlType.insert("QLineEdit", enumCTQLineEdit);
        mapCtrlType.insert("QTextEdit", enumCTQTextEdit);
        mapCtrlType.insert("QPlainTextEdit", enumCTQPlainTextEdit);
        mapCtrlType.insert("QPushButton", enumCTQPushButton);
        mapCtrlType.insert("QLabel", enumCTQLabel);
        mapCtrlType.insert("QDial", enumCTQDial);
        mapCtrlType.insert("QDateEdit", enumCTQDateEdit);
        mapCtrlType.insert("QTimeEdit", enumCTQTimeEdit);
        mapCtrlType.insert("QDateTimeEdit", enumCTQDateTimeEdit);
        mapCtrlType.insert("QSpinBox", enumCTQSpinBox);
        mapCtrlType.insert("QDoubleSpinBox", enumCTQDoubleSpinBox);
        //ctrlShowInit(inWidgetName);
    }
}

CtrlShow::~CtrlShow()
{
}

bool CtrlShow::connectSQLiteDB(const QString &nameDB)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(nameDB);

    //打开数据库
    bDBConnetcted = true;
    if(!database.open())
    {
        //qDebug() << database.lastError();
        //qFatal("failed to connect!") ;
        bDBConnetcted = false;
    }
    return bDBConnetcted;
}

void CtrlShow::getSQLiteField(const QString &nameTable)
{

    //取表字段信息
    QSqlQuery sqlQuery;
    QString sqlSelect = QString("pragma table_info ('%1')").arg(nameTable);
    //qDebug() << sqlSelect;
    if(!sqlQuery.exec(sqlSelect))
    {
        //qDebug() << sqlQuery.lastError();
        return;
    }

    hashDBField.clear();
    int hashDBFieldOrder = 0;                        //KEY
    QString hashDBFieldName;                     //VALUE

    while(sqlQuery.next())
    {
        hashDBFieldName = sqlQuery.value(1).toString();
        hashDBField.insert(hashDBFieldOrder, hashDBFieldName);
        ++hashDBFieldOrder;
    }
     iFieldTotal = hashDBField.count();

//    QHash<int, QString>::const_iterator j;
//    for (j = hashDBField.constBegin(); j != hashDBField.constEnd(); ++j)
//    {
//        qDebug() << j.key() << ":" << j.value();
//    }

//    for(int i = 0; i <  iFieldTotal; ++i)
//    {
//        qDebug() << i << hashDBField.value(i);
//    }

}

void CtrlShow::ctrlShowInit(const QString &strInWName)
{
    //==构建sql语句/可根据需要读取不同的字段==
    QSqlQuery sqlQuery;
    //QString sqlSelect = QString("SELECT iDispNo, strUIType, strLabelString, iDRowLabel, iDColumnLabel, iSpanRowLabel, iSpanColumnLabel, strCtrlType, strCtrlObjName, strDispString, iDRow, iDColumn, iSpanRow, iSpanColumn  FROM UIInfo  WHERE widgetname = '%1'  ORDER BY iDispNo").arg(strInWName);
    //("iDispNo", "strUIType", "strLabelString", "iDRowLabel", "iDColumnLabel", "iSpanRowLabel", "iSpanColumnLabel", "strCtrlType", "strCtrlObjName", "strDispString", "iDRow", "iDColumn", "iSpanRow", "iSpanColumn")

    QString sqlSelect = QString("SELECT * FROM UIInfo  ");
    sqlSelect += QString("WHERE widgetname = '%1'  ORDER BY iDispNo").arg(strInWName);
    //qDebug() << sqlSelect;

    if(!sqlQuery.exec(sqlSelect))
    {
        qDebug() << sqlQuery.lastError();
        return;
    }
    //--------------------------------------------------------
    int iRowMargin = 15;                          //Layout边宽
    int iColumnTotalCount = 15;             //网格总数

    gridLayout = new QGridLayout;                            //字段Layout
    gcLayout = new QGridLayout;                               //命令按钮Layout

    //设置列宽
    for(int i = 0; i < iColumnTotalCount; i ++)
    {
        gridLayout->setColumnStretch(i, 1);
        gcLayout->setColumnStretch(i, 1);
    }

    //设置Layout边宽
    gridLayout->setMargin(iRowMargin);
    gcLayout->setMargin(iRowMargin);

    //设置行高
    //gridLayout->setColumnMinimumWidth(2, 15);

    DisplayData inputData;

    //==循环读取数据库表赋值GridLayout==
    while(sqlQuery.next())
    {
        //iDispNo, strUIType, strCtrlType, strLabelString, iDRowLabel, iDColumnLabel, iSpanRowLabel, iSpanColumnLabel, strCtrlObjName, strDispString, iDRow, iDColumn, iSpanRow, iSpanColumn
        inputData.iDispNo = sqlQuery.value("iDispNo").toInt();     //可以不要
        inputData.strUIType = sqlQuery.value("strUIType").toString();
        inputData.strLabelString = sqlQuery.value("strLabelString").toString();
        inputData.iDRowLabel = sqlQuery.value("iDRowLabel").toInt();
        inputData.iDColumnLabel = sqlQuery.value("iDColumnLabel").toInt();
        inputData.iSpanRowLabel = sqlQuery.value("iSpanRowLabel").toInt();
        inputData.iSpanColumnLabel = sqlQuery.value("iSpanColumnLabel").toInt();
        inputData.strCtrlType = sqlQuery.value("strCtrlType").toString();
        inputData.strCtrlObjName = sqlQuery.value("strCtrlObjName").toString();    //创建后用setObjectName，不知道有没有直接方法
        inputData.strDispString = sqlQuery.value("strDispString").toString();
        inputData.iDRow = sqlQuery.value("iDRow").toInt();
        inputData.iDColumn = sqlQuery.value("iDColumn").toInt();
        inputData.iSpanRow = sqlQuery.value("iSpanRow").toInt();
        inputData.iSpanColumn = sqlQuery.value("iSpanColumn").toInt();

        //qDebug() <<inputData.strCtrlType <<"type: " << mapCtrlType << inputData.strCtrlType << mapCtrlType.value(inputData.strCtrlType);
        //通过上面读取的数据库表数据依次生成界面元素
        //数据格式为： 文字标签 + 数据控件
        //先处理文字标签
        if(!inputData.strLabelString.isEmpty())
        {
            QLabel *newQLabel = new QLabel(inputData.strLabelString);
            gridLayout->addWidget(newQLabel, inputData.iDRowLabel, inputData.iDColumnLabel, inputData.iSpanRowLabel,inputData.iSpanColumnLabel );
        }
        //再处理数据控件
        switch (mapCtrlType.value(inputData.strCtrlType))
        {
        case enumCTQComboBox:
        {
            QComboBox *newComboBox = new QComboBox;
            QStringList texts;
            if(inputData.strDispString.left(1) == "1")
            {
                inputData.strDispString.remove(0, 1);
                texts = inputData.strDispString.split(",");
            }
            else
            {
                inputData.strDispString.remove(0, 1);
                texts = fillDBList(strInWName, inputData.strCtrlObjName).split(",");
            }
            newComboBox->addItems(texts);
            newComboBox->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newComboBox, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQFontComboBox:
        {
            QFontComboBox *newFontComboBox = new QFontComboBox;
            newFontComboBox->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newFontComboBox, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQLineEdit:
        {
            QLineEdit *newQLineEdit = new QLineEdit;
            newQLineEdit->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQLineEdit, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQTextEdit:
        {
            QTextEdit *newQTextEdit = new QTextEdit;
            newQTextEdit->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQTextEdit, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQPlainTextEdit:
        {
            QPlainTextEdit *newQPlainTextEdit = new QPlainTextEdit;
            newQPlainTextEdit->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQPlainTextEdit, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQDateEdit:
        {
            QDateEdit *newQDateEdit = new QDateEdit;
            newQDateEdit->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQDateEdit, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQTimeEdit:
        {
            QTimeEdit *newQTimeEdit = new QTimeEdit;
            newQTimeEdit->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQTimeEdit, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQDateTimeEdit:
        {
            QDateTimeEdit *newQDateTimeEdit = new QDateTimeEdit;
            newQDateTimeEdit->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQDateTimeEdit, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQDial:
        {
            QDial *newQDial = new QDial;
            newQDial->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQDial, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQSpinBox:
        {
            QSpinBox *newQSpinBox = new QSpinBox;
            newQSpinBox->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQSpinBox, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        case enumCTQDoubleSpinBox:
        {
            QDoubleSpinBox *newQDoubleSpinBox = new QDoubleSpinBox;
            newQDoubleSpinBox->setObjectName(inputData.strCtrlObjName);
            gridLayout->addWidget(newQDoubleSpinBox, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        //设置命令按钮行 gcLayout
        case enumCTQPushButton:
        {
            QPushButton *newPushButton = new QPushButton(inputData.strDispString);
            newPushButton->setObjectName(inputData.strCtrlObjName);
            gcLayout->addWidget(newPushButton, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }

        default:
            //错误处理
            qDebug() << inputData.strCtrlType;
            QLabel *newQLabel = new QLabel("Error!");
            newQLabel->setObjectName(inputData.strCtrlObjName);
            if(inputData.strUIType =="UI")
                gridLayout->addWidget(newQLabel, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            else
                gcLayout->addWidget(newQLabel, inputData.iDRow, inputData.iDColumn, inputData.iSpanRow,inputData.iSpanColumn );
            break;
        }
    }

    //最后用垂直布局把字段Layout和命令按钮Layout封装在一起
    QVBoxLayout *thisLayout = new QVBoxLayout(this);
    thisLayout->addLayout(gridLayout);
    thisLayout->addLayout(gcLayout);
}

//取ComboBox内容
QString CtrlShow::fillDBList(const QString &strInWName, const QString &strComboName)
{
    QSqlQuery sqlQueryCombo;
    //读取字段，可根据需要读取不同的字段
    QString sqlCombo = QString("SELECT * FROM combolist ");
    sqlCombo += QString("WHERE strWidget = '%1' AND strComboName = '%2' ORDER BY IDComboList").arg(strInWName).arg(strComboName);
    
    //qDebug() << sqlCombo;
    QString strCombo = "";
    if(!sqlQueryCombo.exec(sqlCombo))
    {
        qDebug() << sqlQueryCombo.lastError();
    }
    else
    {
        while(sqlQueryCombo.next())
        {
            strCombo.append(sqlQueryCombo.value("strListString").toString());
            strCombo.append(",");
        }
        strCombo.remove(-1, 1);            //删除最后一个","
        //qDebug() << strCombo;
    }
    return strCombo;
}
