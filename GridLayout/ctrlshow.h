#ifndef CTRLSHOW_H
#define CTRLSHOW_H

#include <QWidget>
#include <QHash>
#include <QMap>
#include <QGridLayout>

class CtrlShow : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlShow(QWidget *parent = 0);
    ~CtrlShow();
    void ctrlShowInit(const QString &strInWName);         //初始化UI控件

private:
    bool connectSQLiteDB(const QString &);                      //链接数据库
    void getSQLiteField(const QString &);                           //获取表字段
    QString fillDBList(const QString &, const QString &);   //获取ComboBox项目

private:
    QMap<QString, int> mapCtrlType;          //控件名称列表
    QHash<int, QString> hashDBField;           //保存字段名字和顺序
    int iFieldTotal;                                               //字段计数
    bool bDBConnetcted;                                 //数据库链接成功标识
    QString m_inWidgetName;                       //目标窗口名字
    QGridLayout *gridLayout;
    QGridLayout *gcLayout;

};

#endif // CTRLSHOW_H
