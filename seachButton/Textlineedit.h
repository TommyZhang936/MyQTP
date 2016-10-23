#ifndef TextlLineEdit_H
#define TextlLineEdit_H

#include <QLineEdit>

class QLabel;
class QString;

class TextLineEdit : public QLineEdit
{
    Q_OBJECT
    
public:
    TextLineEdit(const QString &text);
    ~TextLineEdit();

    void setLabelStyle(const QString &styleLabel);
    void setLabelWidth(const int widthLabel);

private:
    QLabel * textLabel;
    
};
#endif // TextlLineEdit_H
