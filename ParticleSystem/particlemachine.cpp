#include "particlemachine.h"

ParticleMachine::ParticleMachine(QObject *parent) :
    QObject(parent)
{
    m_roomRect = QRect(0,0,100,100);
}

void ParticleMachine::emitParticle(Particle particle)
{
    m_particles.push(particle);
}

void ParticleMachine::setRoomRect(QRect rect)
{
    m_roomRect = rect;
}

void ParticleMachine::step()
{
    for(int i = 0;i < m_particles.count();i++)
    {
        Particle *ptParticle = &m_particles[i];
        //要开启碰撞检测,将下面的注释去掉:
        //checkBound(ptParticle);

        //阻尼效果控制 - 开始
        if(ptParticle->position().y() >= m_roomRect.y() + m_roomRect.height() - 5)
            ptParticle->reduceVelocity(true);
        else
            ptParticle->reduceVelocity(false);
        //结束

        //要开启黑洞吸引效果,将下面的注释去掉:
        //ptParticle->assignExtraVelocity((QVector2D(m_roomRect.width() / 2.0,m_roomRect.height() / 2.0) - ptParticle->position()) / 2.0);
        ptParticle->step();

        if(ptParticle->life() == 0)
        {
            m_particles[i] = m_particles[m_particles.count() - 1];
            m_particles.pop();
        }
    }
}

void ParticleMachine::checkBound(Particle *particle)
{
    if(particle->position().x() - 5 < m_roomRect.x() || particle->position().x() + 5 > m_roomRect.x() + m_roomRect.width())
        particle->reverseVx();
    if(particle->position().y() - 5 < m_roomRect.y() || particle->position().y() + 5 > m_roomRect.y() + m_roomRect.height())
        particle->reverseVy();
}

void ParticleMachine::render(QPainter &p)
{
    p.setPen(QColor(Qt::red));
    p.setBrush(QBrush(QColor(255,0,0,180)));
    //p.setBrush(QColor(rand() % 256, rand() % 256, rand() % 256, rand() % 256));
    for(int i = 0;i < m_particles.count();i++)
    {
        //要开启拖尾效果,将下面的注释去掉:
        for(int j = 0;j < m_particles[i].tails().count();j++)
        {
            qreal tailOpacity = 1.0 - (j + 1.0) / m_particles[i].tails().count();
            p.save();
            p.translate(m_particles[i].tails()[j].pos.toPointF());
            p.setOpacity(-tailOpacity + m_particles[i].life() / m_particles[i].age());
            p.setPen(Qt::NoPen);
            p.setBrush(QBrush(Qt::gray));
            p.drawEllipse(-2,-2,4,4);
            p.restore();
        }

        p.save();
        p.translate(m_particles[i].position().toPointF());
        p.setOpacity(m_particles[i].life() / m_particles[i].age());
        p.rotate(m_particles[i].rotate());
        p.drawEllipse(-7,-7,14,14);
        p.drawLine(0,0,4,4);
        p.restore();
    }
}
