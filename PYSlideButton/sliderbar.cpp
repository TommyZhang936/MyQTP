#include "sliderbar.h"
#include "qevent.h"
#include "qpainter.h"
#include "qtimer.h"
#include "qdebug.h"

SliderBar::SliderBar(QWidget *parent) :	QWidget(parent)
{
	currentIndex = -1;
	currentItem = "";

	bgColorStart = QColor(121, 121, 121);
	bgColorEnd = QColor(78, 78, 78);
	old_bgColorEnd = bgColorEnd;

	barColorStart = QColor(46, 132, 243);
	barColorEnd = QColor(39, 110, 203);
	old_barColorEnd = barColorEnd;

    textNormalColor = QColor(230, 230, 230);
	textSelectColor = QColor(255, 255, 255);

	radius = 5;
	space = 30;
	fixed = true;
	keyMove = false;
	horizontal = true;
	flat = false;
	itemRepeat = false;

	isForward = true;
	isVirgin = true;

    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(slide()));
}

SliderBar::~SliderBar()
{
    if (timer->isActive()) {
        timer->stop();
	}
}

void SliderBar::resizeEvent(QResizeEvent *)
{
	int index = 0;
	int count = listItem.count();

	if (count == 0) {
		return;
	}

	if (count > 0 && currentItem.isEmpty()) {
		currentIndex = 0;
		currentItem = listItem.at(0).first;
	}

	for (int i = 0; i < count; i++) {
		if (listItem.at(i).first == currentItem) {
			index = i;
			break;
		}
	}

	moveTo(index);
}

void SliderBar::mousePressEvent(QMouseEvent *e)
{
	moveTo(e->pos());
}

void SliderBar::keyPressEvent(QKeyEvent *keyEvent)
{
	if (!keyMove) {
		return;
	}

	if (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Up) {
		movePrevious();
	} else if (keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Down) {
		moveNext();
	}
}

void SliderBar::paintEvent(QPaintEvent *)
{
    //绘制准备工作,启用反锯齿
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	//绘制背景色
	drawBg(&painter);
	//绘制当前条目选中背景
	drawBar(&painter);
	//绘制条目文字
	drawText(&painter);
}

void SliderBar::drawBg(QPainter *painter)
{
	painter->save();
	painter->setPen(Qt::NoPen);
	QLinearGradient bgGradient(QPoint(0, 0), QPoint(0, height()));
	bgGradient.setColorAt(0.0, bgColorStart);
	bgGradient.setColorAt(1.0, bgColorEnd);
	painter->setBrush(bgGradient);
	painter->drawRoundedRect(rect(), radius, radius);
	painter->restore();
}

void SliderBar::drawBar(QPainter *painter)
{
	painter->save();
	painter->setPen(Qt::NoPen);
	QLinearGradient barGradient(barRect.topLeft(), barRect.bottomLeft());
	barGradient.setColorAt(0.0, barColorStart);
	barGradient.setColorAt(1.0, barColorEnd);
	painter->setBrush(barGradient);
	painter->drawRoundedRect(barRect, radius, radius);
	painter->restore();
}

void SliderBar::drawText(QPainter *painter)
{
	painter->save();

	QFont textFont;
	textFont.setBold(true);
	painter->setFont(textFont);

	int count = listItem.count();
    double init = 0;
    double textLen = 0;
	QString strText;

	//逐个绘制元素队列中的文字及文字背景
	for (int i = 0; i < count; i++) {
		strText = listItem.at(i).first;
		textLen  = fontMetrics().width(strText);
		QPointF left(init, 0);
		QPointF right(init + textLen + space, height());

		if (!horizontal) {
			textLen  = fontMetrics().height();
			left = QPointF(0, init);
			right = QPointF(width(), init + textLen + space);
		}

		QRectF textRect(left, right);

		listItem[i].second = textRect;

		if (isVirgin) {
			barRect = textRect;
			isVirgin = false;
		}

        //当前选中区域的文字显示选中文字颜色
		if (textRect == listItem.at(currentIndex).second) {
			painter->setPen(textSelectColor);
		} else {
			painter->setPen(textNormalColor);
		}

		painter->drawText(textRect, Qt::AlignCenter, strText);
		init += textLen + space;
	}

	//设置固定宽度
	if (fixed) {
		if (horizontal) {
            setFixedWidth(init);
		} else {
            setFixedHeight(init);
		}
	}

	painter->restore();
}

int SliderBar::initStep(int distance)
{
	int n = 1;

	while (1) {
		if (n * n > distance) {
			break;
		} else {
			n++;
		}
	}

	return n * 1.4;
}

void SliderBar::slide()
{
	if (step > 1) {
		step--;
	}

	if (horizontal) {
		barLen = barRect.topLeft().x();
	} else {
		barLen = barRect.topLeft().y();
	}

	if (isForward) {
		barLen += step;

		if (barLen >= targetLen) {
			barLen = targetLen;
            timer->stop();
		}
	} else {
		barLen -= step;

		if (barLen <= targetLen) {
			barLen = targetLen;
            timer->stop();
		}
	}

	if (horizontal) {
		barRect = QRectF(QPointF(barLen, 0), QPointF(barLen + barRect.width(), height()));
	} else {
		barRect = QRectF(QPointF(0, barLen), QPointF(width(), barLen + barRect.height()));
	}

	update();
}

void SliderBar::addItem(QString item)
{
	if (!itemRepeat) {
		int count = listItem.count();

		//重复的元素不添加(除重复的空元素外),保证元素的唯一性
		for (int i = 0; i < count; i++) {
			if (listItem.at(i).first == item && !item.trimmed().isEmpty()) {
				return ;
			}
		}
	}

	listItem.push_back(qMakePair(item, QRectF()));
	update();
}

void SliderBar::clearItem()
{
	listItem.clear();
	update();
}

void SliderBar::setBarColor(QColor barColorStart, QColor barColorEnd)
{
	this->barColorStart = barColorStart;
	this->barColorEnd = barColorEnd;
	this->old_barColorEnd = barColorEnd;
	update();
}

void SliderBar::setBgColor(QColor bgColorStart, QColor bgColorEnd)
{
	this->bgColorStart = bgColorStart;
	this->bgColorEnd = bgColorEnd;
	this->old_bgColorEnd = bgColorEnd;
	update();
}

void SliderBar::setTextColor(QColor textNormalColor, QColor textSelectColor)
{
	this->textNormalColor = textNormalColor;
	this->textSelectColor = textSelectColor;
	update();
}

void SliderBar::setRadius(int radius)
{
	this->radius = radius;
	update();
}

void SliderBar::setSpace(int space)
{
	this->space = space;
	update();
}

void SliderBar::setFixed(bool fixed)
{
	this->fixed = fixed;
	update();
}

void SliderBar::setKeyMove(bool keyMove)
{
	this->keyMove = keyMove;

	if (keyMove) {
		setFocusPolicy(Qt::StrongFocus);
	} else {
		setFocusPolicy(Qt::NoFocus);
	}
}

bool SliderBar::setHorizontal(bool horizontal)
{
	this->horizontal = horizontal;
	update();
}

void SliderBar::setFlat(bool flat)
{
	//扁平后将初始颜色赋值给结束颜色达到扁平的效果,如果取消扁平则再次恢复原有的颜色
	if (flat) {
		bgColorEnd = bgColorStart;
		barColorEnd = barColorStart;
	} else {
		bgColorEnd = old_bgColorEnd;
		barColorEnd = old_barColorEnd;
	}

	this->flat = flat;
	update();
}

void SliderBar::setItemRepeat(bool itemRepeat)
{
	this->itemRepeat = itemRepeat;
}

void SliderBar::moveFirst()
{
	int index = 0;

	if (currentIndex != index) {
		moveTo(index);
	}
}

void SliderBar::moveLast()
{
	int index = listItem.count() - 1;

	if (currentIndex != index) {
		moveTo(index);
	}
}

void SliderBar::movePrevious()
{
	if (currentIndex > 0) {
		currentIndex--;
		moveTo(currentIndex);
	}
}

void SliderBar::moveNext()
{
	if (currentIndex < listItem.count() - 1) {
		currentIndex++;
		moveTo(currentIndex);
	}
}

void SliderBar::moveTo(int index)
{
	if (listItem.count() > index) {
		QRectF rec = listItem.at(index).second;
		QPoint pos = QPoint(rec.x(), rec.y());
		moveTo(pos);
	}
}

void SliderBar::moveTo(QString item)
{
	int count = listItem.count();

	for (int i = 0; i < count; i++) {
		if (listItem.at(i).first == item) {
			moveTo(i);
			break;
		}
	}
}

void SliderBar::moveTo(QPointF point)
{
	int count = listItem.count();

	for (int i = 0; i < count; i++) {
		if (listItem.at(i).second.contains(point)) {
			currentIndex = i;
			currentItem = listItem.at(i).first;
			emit currentItemChanged(currentIndex, currentItem);

			targetRect = listItem.at(i).second;

			if (horizontal) {
				targetLen = targetRect.topLeft().x();
				barLen = barRect.topLeft().x();
			} else {
				targetLen = targetRect.topLeft().y();
				barLen = barRect.topLeft().y();
			}

			isForward = (targetLen > barLen);
			int distance = targetLen - barLen;
			distance = qAbs(distance);

			//重新获取每次移动的步长
			step = initStep(distance);
            timer->start();
		}
	}
}
