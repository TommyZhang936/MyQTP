#ifndef QPROGRESSINDICATOR_H
#define QPROGRESSINDICATOR_H

#include <QWidget>
#include <QColor>

class QProgressIndicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int delay READ animationDelay WRITE setAnimationDelay)
    Q_PROPERTY(bool displayedWhenStopped READ isDisplayedWhenStopped WRITE setDisplayedWhenStopped)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    
public:
    QProgressIndicator(QWidget* parent = 0);

    /* Returns the delay between animation steps.
        The number of milliseconds between animation steps. By default, the animation delay is set to 40 milliseconds.
        setAnimationDelay
     */
    int animationDelay() const { return m_delay; }

    /* Returns a Boolean value indicating whether the component is currently animated.
        Animation state.
        startAnimation stopAnimation
     */
    bool isAnimated () const;

    /*Returns a Boolean value indicating whether the receiver shows itself even when it is not animating.
       Return true if the progress indicator shows itself even when it is not animating. By default, it returns false.
       setDisplayedWhenStopped
     */
    bool isDisplayedWhenStopped() const;

    /*Returns the color of the component.
        setColor
      */
    const QColor & color() const { return m_color; }

    virtual QSize sizeHint() const;
    int heightForWidth(int w) const;
    
public slots:
    /*Starts the spin animation.
        stopAnimation isAnimated
     */
    void startAnimation();

    /*Stops the spin animation.
        startAnimation isAnimated
     */
    void stopAnimation();

    /*Sets the delay between animation steps.
        Setting the delay to a value larger than 40 slows the animation, while setting the delay to a smaller value speeds it up.
        param delay The delay, in milliseconds. 
        animationDelay 
     */
    void setAnimationDelay(int delay);

    /*Sets whether the component hides itself when it is not animating. 
       param state The animation state. Set false to hide the progress indicator when it is not animating; otherwise true.
       isDisplayedWhenStopped
     */
    void setDisplayedWhenStopped(bool state);

    /*Sets the color of the components to the given color.
        color
     */
    void setColor(const QColor & color);
    
protected:
    virtual void timerEvent(QTimerEvent * event); 
    virtual void paintEvent(QPaintEvent * event);
    
private:
    int m_angle;
    int m_timerId;
    int m_delay;
    bool m_displayedWhenStopped;
    QColor m_color;
};

#endif // QPROGRESSINDICATOR_H
