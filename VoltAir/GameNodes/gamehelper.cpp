#include "gamehelper.h"
#include "Box2dUtil.h"
#include "Body.h"
#include "Actor.h"
#include "inventory/ActorWeapon.h"

GameHelper* GameHelper::mInstance = nullptr;

GameHelper* GameHelper::getInstance()
{
    if(!mInstance){
        mInstance = new GameHelper;
    }
    return mInstance;
}

GameHelper::GameHelper(QObject *parent) : QObject(parent)
{

}

GameHelper::~GameHelper()
{
}

bool GameHelper::canCollide(unsigned int A, unsigned int B){
    return Box2dUtil::canCollide(A,B);
}

unsigned int GameHelper::collideWithAllBut(unsigned int which)
{
    return Box2dUtil::collideWithAllBut(which);
}

unsigned int GameHelper::getActorCollisionCategory(Actor *actor)
{
    unsigned int category = 0;
    Body* body = actor->getBody();

    if(body){
        category = body->getCategoryBits();
    }
    return category;
}

unsigned int GameHelper::getActorCollisionMask(Actor *actor)
{
    unsigned int mask = 0;
    Body* body = actor->getBody();

    if(body){
        mask = body->getCategoryBits();
    }
    return mask;
}


ContactManifold* GameHelper::getContactManifold(Actor *actor)
{
    ContactManifold* manifold = nullptr;
    Body* body = actor->getBody();

    if(body){
        manifold = body->getContactManifold();
    }
    return manifold;
}
