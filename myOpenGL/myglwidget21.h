#ifndef MYGLWIDGET_H  
#define MYGLWIDGET_H  
  
#include <QWidget>  
#include <QGLWidget>

class QSound;  

class MyGLWidget : public QGLWidget
{  
    Q_OBJECT  
    
public:  
    explicit MyGLWidget(QWidget *parent = 0);  
    ~MyGLWidget();  
      
protected:  
    //对3个纯虚函数的重定义  
    void initializeGL();  
    void resizeGL(int w, int h);  
    void paintGL();  
      
    void keyPressEvent(QKeyEvent *event);           //处理键盘按下事件  
      
private:  
    void resetObjects();                            //重置玩家和敌人信息  
    void updateData();                              //更新下一帧数据  
    void buildFont();                               //创建字体  
    void killFont();                                //删除显示列表  
    //输出字符串  
    void glPrint(GLuint x, GLuint y, int set, const char *fmt, ...);  
  
private:  
    bool fullscreen;                                //是否全屏显示  
    bool m_Vline[11][10];                           //保存垂直方向的11根线条中,每根线条中的10段是否被走过  
    bool m_Hline[10][11];                           //保存水平方向的11根线条中,每根线条中的10段是否被走过  
    bool m_Filled;                                  //网格是否被填满  
    bool m_Gameover;                                //游戏是否结束  
    bool m_Anti;                                    //是否反走样  
  
    int m_Delay;                                    //敌人的暂停时间  
    int m_Adjust;                                   //调整速度  
    int m_Lives;                                    //玩家的生命  
    int m_Level;                                    //内部的游戏难度等级  
    int m_Level2;                                   //显示的游戏难度等级  
    int m_Stage;                                    //游戏的关卡  
    static const int s_Steps[6];                    //用来调节显示的速度  
  
    struct object                                   //记录游戏中的对象  
    {  
        int fx, fy;                                 //使移动变得平滑  
        int x, y;                                   //当前游戏者的位置  
        float spin;                                 //旋转角度  
    };  
    object m_Player;                                //玩家信息  
    object m_Enemy[9];                              //最多9个敌人  
    object m_Hourglass;                             //宝物沙漏信息  
  
    QString m_FileName[2];                          //图片的路径及文件名  
    GLuint m_Texture[2];                            //储存两个纹理  
    GLuint m_Base;                                  //字符显示列表的开始值  
    QSound *m_Sound;                                //保存吃到宝物后的计时音乐  
    
};  
  
#endif // MYGLWIDGET_H 
