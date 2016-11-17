#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector2D>
#include <QQueue>

struct PTail
{
    QVector2D pos;
    int rotate;
};

class Particle
{
public:
    Particle();
    void setProperties(QVector2D pos,QVector2D velo,QVector2D acc,qreal life);
    void assignExtraVelocity(QVector2D exV);
    QVector2D position();
    QVector2D velocity();
    qreal life();
    qreal age();
    qreal rotate();
    QQueue<PTail> tails();
    void reverseVx() { m_velocity.setX(-m_velocity.x()); }
    void reverseVy() { m_velocity.setY(-m_velocity.y()); }
    void reduceVelocity(bool type);

    void step();

private:
    QVector2D m_position;
    QVector2D m_velocity;
    QVector2D m_exVelocity;
    QVector2D m_acceleration;
    QQueue<PTail> m_tails;
    qreal m_life;
    qreal m_age;
    qreal m_rotate;

};

#endif // PARTICLE_H
