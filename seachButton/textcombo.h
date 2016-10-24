#ifndef TEXTCOMBO_H
#define TEXTCOMBO_H

#include <QComboBox>

class QLabel;
class QString;

class TextCombo : public QComboBox
{
    Q_OBJECT
    
public:
    TextCombo(const QString &text);
    ~TextCombo();

    void setLabelStyle(const QString &styleLabel);
    void setLabelWidth(const int widthLabel);

private:
    QLabel * textLabel;
    
};

#endif // TEXTCOMBO_H
