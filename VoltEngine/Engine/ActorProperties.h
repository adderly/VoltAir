#ifndef ACTORPROPERTIES_H
#define ACTORPROPERTIES_H

#include <QObject>
#include <QPointF>


/**
*   @brief This class holds the Actor's basic information, for having
*    them live and run freely in the memory.
*   @note This is not really extensible, and it might be to specific to my current needs.
*   TODO: add qml custom properties capability, so this can be done by scripting. The
*   drawback would be speed cause will have to do some migthy search. Maybe some genius
*   wants to solve this.
*   Requirement: Parse custom qml properties, and add them in some set or hashmap.
*                Adding the ability for users to manage this as desired.
*/
class ActorProperties:public QObject
{
    Q_OBJECT

    Q_PROPERTY(float health READ health WRITE setHealth NOTIFY healthChanged)
    Q_PROPERTY(float maxSpeed READ maxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)

public:
    ActorProperties(QObject* parent = nullptr);
    ~ActorProperties();

    enum ACTOR_CLAN:unsigned int{
        GOOD = 1,
        BAD
    };

    /**
    *   @brief Sets the #health.
    */
    void setHealth(float h);

    /**
    *   @brief Returs the #health.
    */
    float health() {  return mHealth;  }

    /**
    *   @brief Set the #maxspeed.
    */
    void setMaxSpeed(float s);

    /**
    *   @brief Gets the #maxspeed.
    */
    float maxSpeed() {  return mMaxSpeed;  }

    /**
    *   @brief Sets the #forwardVector.
    *   @note This is used for controlling actor
    * direction when moving or whatever.
    */
    void setForwardVector(QPointF f);

    /**
    *   @brief Return the #forwardVector.
    */
    QPointF forwardVector() {  return mForwardVector;  }

    float mHealth = 0.0f;
    float mMaxSpeed = 0.0f;
    QPointF mForwardVector;

signals:
    void healthChanged();
    void maxSpeedChanged();
    void forwardVectorChanged();

private:
};
Q_DECLARE_METATYPE(ActorProperties*)

#endif // ACTORPROPERTIES_H
