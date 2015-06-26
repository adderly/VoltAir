#include "ActorWeapon.h"
#include "Actor.h"
#include "bodies/CircleBody.h"
#include "utils/Box2dUtil.h"
#include "utils/Util.h"
#include "Box2D.h"

ActorWeapon::ActorWeapon(QObject* parent):
    QObject(parent)
{

}

void ActorWeapon::setAttackDistance(float value){
    mAttackDistance = value;
    emit attackDistanceChanged();
}

void ActorWeapon::setAutoReload(bool value){
    mAutoreload = value;
    emit autoReloadChanged();
}

void ActorWeapon::setOwner(Actor* value){
    mOwner = value;
}

void ActorWeapon::attack()
{
    Util::javaScriptExecuteFuncIfExists(this,"attackFunc");
}

Actor* ActorWeapon::createBullet(BulletType type)
{
    CircleBody* body = new CircleBody();
    body->setProperty("type",type);
    body->setRadius(.1f);
    body->setBullet(true);
    if(mOwner){
        body->setCategoryBits(mOwner->getBody()->getCategoryBits());
        body->setMaskBits(mOwner->getBody()->getMaskBits());
        body->setGroupIndex(mOwner->getBody()->getGroupIndex());
        body->setPosition(mOwner->position());
        body->setInitialX(mOwner->position().x());
        body->setInitialY(mOwner->position().y());
    }

    b2BodyDef def = body->getBodyDef();
    def.type = static_cast<b2BodyType>(Body::DynamicBody);
    //body->setPosition(mOwner->position());
    body->initBody(def);
    qDebug() << "owner Position = " << mOwner->position();

    Actor* bullet = new Actor(static_cast<Body*>(body));

    bullet->setPosition(mOwner->position());

    return bullet;
}
