#ifndef MONEYTOHZ_H
#define MONEYTOHZ_H

#include <QString>
#include <QHash>

class MoneyToHZ 
{    
public:
    MoneyToHZ();
    ~MoneyToHZ();

    QString getHZ(const double);
    
private:
    QHash<int, QString> strHZ09;
    QHash<int, QString> strInteger;
    QHash<int, QString> strIntegerZ;
    QHash<int, QString> strDecimal;
    
    int maxCount;
};

#endif // MONEYTOHZ_H
