#ifndef MAINDLG_H
#define MAINDLG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class MainDlg;
}

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MainDlg(QWidget *parent = 0);
    ~MainDlg();

    bool connectSQLiteDB(const QString);
    void fillColorList(QComboBox *);
    void fillDBList(QComboBox *);
    void Init();

public slots:
    void showColor();
    void showDB();

private:
    Ui::MainDlg *ui;

    QComboBox *colorCombo;
    QComboBox *dbCombo;

    QMap <QString, QString> dbNameMap;
};

#endif // MAINDLG_H
