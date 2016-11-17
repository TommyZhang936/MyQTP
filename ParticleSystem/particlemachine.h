#ifndef PARTICLEMACHINE_H
#define PARTICLEMACHINE_H

#include <QObject>
#include <QStack>
#include <QPainter>

#include "particle.h"

class ParticleMachine : public QObject
{
    Q_OBJECT
public:
    explicit ParticleMachine(QObject *parent = 0);
    void emitParticle(Particle particle);
    void setRoomRect(QRect rect);
    void step();
    void clear() { m_particles.clear(); }

    void render(QPainter &p);
signals:

public slots:

private:
    void checkBound(Particle *particle);

private:
    QStack<Particle> m_particles;
    QRect m_roomRect;
};

#endif // PARTICLEMACHINE_H
