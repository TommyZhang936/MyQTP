#ifndef TEXTTICKER_H
#define TEXTTICKER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class TextTicker : public QWidget
{
	Q_OBJECT

public:
	TextTicker(QWidget *parent = 0);
	~TextTicker();
    void setTickerText(const QString &);
    void startTicker(const int &);
    void setRect(const QRect &);
    void setBGColor(const QColor &);
    void setTickerFont(const int &fsize, const QColor &tcolor, const QString &ffont = "Microsoft YaHei");
    
private:
	void showTicker();

	int m_curIndex;
	QLabel *m_label;
	QString m_showText;
    QTimer *timerTicker;
};

#endif // TEXTTICKER_H
