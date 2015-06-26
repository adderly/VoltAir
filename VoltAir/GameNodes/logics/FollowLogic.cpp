#include "FollowLogic.h"
#include "Body.h"

FollowLogic::FollowLogic(QObject *parent):
    Logic(parent)
{

}

FollowLogic::~FollowLogic()
{

}

void FollowLogic::init(){
    Logic::init();

    Actor* actor = getActor();
    if(actor){
        mBody  = actor->getBody();
    }
}


void FollowLogic::update(){
    if( (mBody = getActor()->getBody())){
        mBody->setLinearVelocity(QPointF(400.0,0));
    }
}

