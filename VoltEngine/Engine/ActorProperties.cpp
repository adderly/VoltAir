#include "ActorProperties.h"

ActorProperties::ActorProperties(QObject* parent):
    QObject(parent)
{

}

ActorProperties::~ActorProperties()
{

}

void ActorProperties::setHealth(float h){
    mHealth = h;
    emit healthChanged();
}


void ActorProperties::setMaxSpeed(float s){
    mMaxSpeed = s;
    emit maxSpeedChanged();
}

void ActorProperties::setClan(unsigned int c)
{
    mClan = c;
}

