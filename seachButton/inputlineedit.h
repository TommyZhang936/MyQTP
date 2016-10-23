#ifndef INPUTLINEEDIT_H
#define INPUTLINEEDIT_H

#include <QLabel>

class QLineEdit;
class QString;

class InputLineEdit : public QLabel
{
    Q_OBJECT
    
public:
    InputLineEdit(const QString &text, QLineEdit *edit);
    
};
#endif // INPUTLINEEDIT_H
