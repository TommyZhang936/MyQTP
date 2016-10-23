#ifndef SEARCHBUTTON_H
#define SEARCHBUTTON_H

#include <QPushButton>

class QLineEdit;
class QString;

class SearchButton : public QPushButton 
{
    Q_OBJECT
    
public:
    SearchButton(const QString &text, QLineEdit *edit);

    void setPBStyle(const QString &stylePB);
    void setPBWidth(const int widthPB);

private:
    QLineEdit* thisEdit;
    
};

#endif // SEARCHBUTTON_H
