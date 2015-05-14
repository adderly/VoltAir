#include "InputLogic.h"
#include "GameNodes/inputs/PlayerManager.h"
#include "Body.h"

InputLogic::InputLogic(QObject* parent):Logic(parent)
{

}

InputLogic::~InputLogic()
{

}

void InputLogic::init()
{
    Logic::init();

    Actor* actor = getActor();
    if (!actor) {
        qDebug() << "Warning: InputLogic created out of the scope of an Actor";
        return;
    } else if (actor->getActorType() != Actor::PlayerActor) {
        qDebug() << "Warning: InputLogic created out of the scope of a Player Actor";
        return;
    }

    Body* body = actor->getBody();
    if (!body) {
        qDebug() << "Body not found for Actor" << actor->property("id").toString();
        return;
    }

}

void InputLogic::update()
{
    Actor* actor = getActor();
    if (actor && actor->getActorType() != Actor::PlayerActor) {
        return;
    }

    Body* body = actor->getBody();
    PlayerManager* manager = PlayerManager::getInstance();
    int playerId = actor->property("playerId").toInt();

    if (manager->isPlayerAvailable(playerId)) {
        // Re-activate the PlayerActor.  This is to service MultiPlayerMode, but since it will
        // have no effect on a SinglePlayer PlayerActor, we re-activate without prejudice.
        actor->setVisible(true);
        if (body) {
            body->setActive(true);
        }
        float horizontal = manager->getHorizontal(playerId);

        if(horizontal!=0){
            body->setLinearVelocity(QPointF(horizontal*10,0));
            qDebug() << "set linear velocity";
        }
        actor->setProperty("direction",QVector2D(horizontal,0));

        //actor->setProperty("inputForce", manager->getHorizontal(playerId));
        //actor->setProperty("magnetized", manager->isMagnetized(playerId));
    }
}

