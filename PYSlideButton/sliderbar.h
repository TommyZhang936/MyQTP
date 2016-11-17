#ifndef SLIDERBAR_H
#define SLIDERBAR_H

/**
 * 作者:feiyangqingyun(QQ:517216493) 2016-10-8
 * 本控件来源于网络(原作者:kimtaikee(http://www.qtcn.org/bbs/read-htm-tid-45439-ds-1.html#tpc))
 * 1:增加键盘按键上下移动元素功能
 * 2:增加窗体大小改变控件自动拉伸支持
 * 3:增加移动到第一个/末一个/上移/下移/移动到指定索引/移动到指定元素
 * 4:增加扁平处理
 * 5:增加纵向风格
 * 6:增加设置圆角接口
 * 7:增加设置间距接口
 * 8:增加设置导航条主背景渐变色
 * 9:增加设置当前条目选中背景渐变色
 */

#include <QWidget>

class SliderBar : public QWidget
{
	Q_OBJECT

public:
	explicit SliderBar(QWidget *parent = 0);
	~SliderBar();

protected:
	void resizeEvent(QResizeEvent *);
	void mousePressEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *);
	void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawBar(QPainter *painter);
    void drawText(QPainter *painter);

private:
    //元素集合,成对出现,元素的名字,矩形区域范围
    QVector<QPair<QString, QRectF> > listItem;

    QRectF barRect, targetRect; //选中区域和目标区域的矩形
    double barLen, targetLen;   //选中区域和目标区域的长度
    int currentIndex;           //当前选中条目索引
    QString currentItem;        //当前选中条目文字

    int step;                   //每次移动的步长
    int initStep(int distance); //计算步长

	QColor bgColorStart;        //导航条主背景渐变开始颜色
	QColor bgColorEnd;          //导航条主背景渐变结束颜色
	QColor barColorStart;       //导航条当前条目渐变开始颜色
	QColor barColorEnd;         //导航条当前条目渐变结束颜色
    QColor textNormalColor;     //文字正常颜色
    QColor textSelectColor;     //文字选中颜色
	QColor old_bgColorEnd;      //用于扁平化切换
	QColor old_barColorEnd;     //用于扁平化切换

	int radius;                 //圆角半径
	int space;                  //条目元素之间的间距
	bool fixed;                 //是否拉伸填充
	bool keyMove;               //是否支持按键移动
	bool horizontal;            //是否横向显示
	bool flat;                  //是否扁平化
	bool itemRepeat;            //条目元素是否可以重复

	bool isForward;             //是否往前移动
    bool isVirgin;              //是否首次处理

    QTimer *timer;              //滑动绘制定时器

private slots:	
	void slide();

public:
	//获取当前条目索引
	int getCurrentIndex()const
	{
		return currentIndex;
	}
	//获取当前选中条目文字
	QString getCurrentItem()const
	{
		return currentItem;
	}

public slots:
	//添加条目,默认会过滤重复条目,如果不要过滤重复条目可以调用 setItemRepeat(true)
    void addItem(QString item);
    //删除所有条目
    void clearItem();

	//设置导航条主背景渐变色
	void setBgColor(QColor bgColorStart, QColor bgColorEnd);
	//设置当前条目选中背景渐变色
	void setBarColor(QColor barColorStart, QColor barColorEnd);

	//设置文字颜色
    void setTextColor(QColor textNormalColor, QColor textSelectColor);
	//设置边框圆角
	void setRadius(int radius);
	//设置条目间距
	void setSpace(int space);
	//设置是否固定宽高
	void setFixed(bool fixed);
	//设置键盘按键是否能够移动条目
	void setKeyMove(bool keyMove);
	//设置横向显示,如果为假则纵向显示
	bool setHorizontal(bool horizontal);
	//设置扁平,默认不扁平
	void setFlat(bool flat);
	//设置条目可否重复,默认不重复
	void setItemRepeat(bool itemRepeat);

	//移动到第一个条目
	void moveFirst();
	//移动到最后一个条目
	void moveLast();
	//往前移动条目
	void movePrevious();
	//往后移动条目
	void moveNext();
	//移动到指定索引条目
	void moveTo(int index);
	//移动到指定文字条目
	void moveTo(QString item);
	//移动到指定坐标位置条目
	void moveTo(QPointF point);

signals:
	//当前条目改变信号,index为当前条目索引索引 item为当前条目文字
	void currentItemChanged(int index, QString item);
};

#endif // SLIDERBAR_H
