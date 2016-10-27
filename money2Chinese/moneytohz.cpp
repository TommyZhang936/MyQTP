#include "moneytohz.h"

#include <QDebug>

MoneyToHZ::MoneyToHZ()
{
    //最大整数位
    maxCount = 64;
    //整数值对应汉字0-9
    QString strCC = "零壹贰叁肆伍陆柒捌玖";
    strHZ09.clear();
    for (int i = 0; i < 10; i++)
    {
        strHZ09.insert(i, strCC.at(i));
    }
    //整数位对应汉字 元-仟万亿顺京
    //理论最大99999999999999999999999999999999999999999999999999999999999999999999999.99
    QString strWW = "元.拾.佰.仟."
                    "万.拾.佰.仟."
                    "亿.拾.佰.仟.万.拾.佰.仟."
                    "兆.拾.佰.仟.万.拾.佰.仟.亿.拾.佰.仟.万.拾.佰.仟."
                    "顺.拾.佰.仟.万.拾.佰.千.亿.拾.佰.仟.兆.拾.佰.仟.万.拾.佰.仟.亿.拾.佰.仟.万.拾.佰.仟.";
                    //"京.拾.佰.仟.万.拾.佰.仟.亿.拾亿.佰亿.仟亿.万亿.";
                    //"顺.拾顺.佰顺.仟顺.万顺.拾万顺.佰万顺.千万顺.亿顺.拾忆顺.佰亿顺.仟亿顺.万亿顺.拾万亿顺.佰万亿顺.仟万亿顺";    
//    QString strWW = "元.拾.佰.仟."
//                    "万.拾.佰.仟."
//                    "亿.拾.佰.仟.万."
//                    "兆.拾.佰.仟.万.拾万.佰万.仟万.亿 拾亿.佰亿.仟亿.万亿."
//                    "顺.拾.佰.仟.万.拾万.佰万.千万.亿.拾忆.佰亿.仟亿.万亿.拾万亿.佰万亿.仟万亿."
//                    "京.拾.佰.仟.万.拾万.佰万.仟万.亿.拾亿.佰亿.仟亿.万亿."
//                    "顺.拾顺.佰顺.仟顺.万顺.拾万顺.佰万顺.千万顺.亿顺.拾忆顺.佰亿顺.仟亿顺.万亿顺.拾万亿顺.佰万亿顺.仟万亿顺";    
    QString strWWZ = "元.拾.佰.仟."
                     "万.拾万.佰万.仟万."
                     "亿.拾亿.佰亿.仟亿.万亿.拾万亿.佰万亿.仟万亿."
                     "兆.拾兆.佰兆.仟兆.万兆.拾万兆.佰万兆.仟万兆.亿兆.拾亿兆.佰亿兆.仟亿兆.万亿兆.拾万亿兆.佰万亿兆.仟万亿兆."
                     "顺.拾顺.佰顺.仟顺.万顺.拾万顺.佰万顺.千万顺.亿顺.拾亿顺.佰亿顺.仟亿顺.万亿顺.拾万亿顺.佰万亿顺.仟万亿顺."
                     "兆顺.拾兆顺.佰兆顺.仟兆顺.万兆顺.拾万兆顺.佰万兆顺.仟万兆顺.亿兆顺.拾亿兆顺.佰亿兆顺.仟亿兆顺.万亿兆顺.拾万亿兆顺.佰万亿兆顺.仟万亿兆顺.";
                    //"京.拾京.佰京.仟京.万京.拾万京.佰万京.仟万京.亿京.拾亿京.佰亿京.仟亿京.万亿京."
                    //"顺京.拾顺京.佰顺京.仟顺京.万顺京.拾万顺京.佰万顺京.千万顺京.亿顺京.拾忆顺京.佰亿顺京.仟亿顺京.万亿顺京.拾万亿顺京.佰万亿顺京.仟万亿顺京"; 
    strInteger.clear();
    strIntegerZ.clear();
    QList<QString> listWW;
    QList<QString> listWWZ;
    listWW = strWW.split('.');
    listWWZ = strWWZ.split('.');
    for(int i = 0; i < listWW.length(); i++)
    {
        strInteger.insert(i, listWW[i]);
        strIntegerZ.insert(i, listWWZ[i]);
    }
    //小数位对应汉字 角分
    strDecimal.clear();
    strDecimal.insert(0, "角");
    strDecimal.insert(1, "分");
    
}

MoneyToHZ::~MoneyToHZ()
{
    
}

QString MoneyToHZ::getHZ(const double input)
{
    QString strOUT ="";
    //转成字符去掉2个以上连续0后面的0
    QString strIN = QString::number(input, 'f', 2);
    //分离整数位和小数位
    QList<QString> listOut = strIN.split('.');
    //处理整数位，先取出位数（因为后面为处理连续0增加的1位不能计入位数）
    int lenInteger = listOut[0].length();
    listOut[0].replace("00", "+0");
    listOut[0].replace("0+", "++");
    listOut[0].replace("00", "+0");
    qDebug() << listOut[0];
    if (lenInteger > maxCount)
    {
        return "数据太大！超界！";
    }
    //为计算 Next 增加一个字符进去
    listOut[0].append('A') ;
    
    strOUT.clear();
    int intWei;
    char charWei;
    char charNext;
    for(int i = 0; i < lenInteger ; i ++ )
    {
        charWei = (listOut[0].at(i)).toLatin1();     
        charNext = (listOut[0].at(i + 1)).toLatin1();     
        if(charWei != '+' )
        {
            //取汉字数字 -零-            
            intWei = (int)(charWei - 48);
            strOUT += strHZ09.value(intWei);
            //取对应位
            if(intWei > 0)
            {
                //strOUT += (charNext == '+') ? strIntegerZ.value(lenInteger - i - 1) : strInteger.value(lenInteger - i - 1);
                strOUT += (intWei == 0) ? strIntegerZ.value(lenInteger - i - 1) : strInteger.value(lenInteger - i - 1);
            }
        }
    }
    //qDebug() << strOUT;
    //处理“零元”
    if(intWei == 0)
    {
        strOUT += "元";
        strOUT.replace("零元", "元");
    }

    //chul处理小数
    //角
    for(int i = 0; i < 2; i++)
    {
        charWei = (listOut[1].at(i)).toLatin1();
        intWei = (int)(charWei - 48);
        strOUT += (intWei == 0) ? "" :  strHZ09.value(intWei) + strDecimal[i];
    }
    strOUT.replace("元角", "元");
    strOUT.replace("元分", "元");
                
    qDebug() << QString::number(input, 'f', 2) << " = " << strOUT;
    
    return strOUT;
}
