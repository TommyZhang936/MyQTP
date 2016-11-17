#include "particle.h"

Particle::Particle()
{

}

void Particle::setProperties(QVector2D pos, QVector2D velo, QVector2D acc, qreal life)
{
    m_position = pos;                               //位置
    m_velocity = velo;                              //速度
    m_acceleration = acc;                        //加速度
    m_life = life;
    m_age = life;
}

void Particle::assignExtraVelocity(QVector2D exV)
{
    m_exVelocity = exV;
}

QVector2D Particle::position()
{
    return m_position;
}

QVector2D Particle::velocity()
{
    return m_velocity;
}

qreal Particle::life()
{
    return m_life;
}

qreal Particle::age()
{
    return m_age;
}

qreal Particle::rotate()
{
    return m_rotate;
}

QQueue<PTail> Particle::tails()
{
     return m_tails;
}

void Particle::step()
{
    if(m_tails.count() >= 25)
        m_tails.dequeue();
    PTail tail;
    tail.pos = m_position;
    tail.rotate = m_rotate;
    m_tails.enqueue(tail);

    m_position += ((m_velocity + m_exVelocity) * 0.1f);
    m_velocity += (m_acceleration * 0.1f);

    m_life -= 0.05;

    if(m_rotate >= 360)
        m_rotate = 0;
    m_rotate += 0.5;

}

void Particle::reduceVelocity(bool type)
{
    qreal vx = m_velocity.x();
    qreal vy = m_velocity.y();

    if(vy < 0)
    {
        vy += 1;
    }

    if(type)
    {
        if(vx < 0)
        {
            vx += 0.01;
        }
        else
        {
            vx -= 0.01;
        }
    }

    m_velocity = QVector2D(vx, vy);
}
