#ifndef GAMEHELPER_H
#define GAMEHELPER_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>

class Actor;
class ContactManifold;

/**
*   @brief Contains enums and helper methods for the game.
*/
class GameHelper : public QObject
{
    Q_OBJECT

    Q_ENUMS(CollisionEntities)
public:
    enum CollisionEntities:unsigned int{
        LEVEL_STRUCTURE = 0x0001,
        FRIENDLY= 0x0002,
        ENEMY= 0x0004,
        COLLECTABLES = 0x0008
    };
    ~GameHelper();

    //  Inventory


    //  Collision Category helper functions.
    /**
    *   @brief Check if A and B can collide.
    */
    Q_INVOKABLE bool canCollide(unsigned int A, unsigned int B);
    /**
    *   @brief Returns a integer of that can collide with everybody but #which.
    */
    Q_INVOKABLE unsigned int collideWithAllBut(unsigned int which);
    /**
    *   @brief Returns the collision category of the passed actor.
    */
    static inline unsigned int getActorCollisionCategory(Actor* actor);
    /**
    *   @brief Returns the collision masks of the passed actor.
    */
    static inline unsigned int getActorCollisionMask(Actor* actor);
    /**
    *   @brief Returns the ContactManifold of a body contained in an Actor.
    */
    static inline ContactManifold* getContactManifold(Actor* actor);

    static GameHelper* getInstance();
private:
    explicit GameHelper(QObject *parent = 0);
    static GameHelper* mInstance;
};

Q_DECLARE_METATYPE(GameHelper*)
#endif // GAMEHELPER_H
