#ifndef ACTORINVENTORY_H
#define ACTORINVENTORY_H

#include <QObject>

class Actor;
class ActorWeapon;
class Pickable;

/**
*   @brief Inventory which will be available for player.
*   @note  For player actors this should be subclassed.
*   The weapons is instantiated in qml, must be placed within a list<QtObject> for being able
*   to search for them inside the ActorInventory because as is a subclass of QObject
*   it does not allow to Custom types as direct childs inside the object.
*   Ex: Inside the Actor this goes like this.
*
*   ActorInventory{
*       id:actor_inventory
*
*        property alias default_weapon:bow2
*
*        property list<QtObject> weapons:[
*           Bow{
*               id:bow2
*           },
*           Bow{
*               id:bow3
*           }
*       ]
*   }
*/
class ActorInventory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ActorWeapon* equipedWeapon READ getEquipedWeapon WRITE setEquipedWeapon NOTIFY equipedWeaponChanged)
public:
    explicit ActorInventory(QObject *parent = 0);
    ~ActorInventory();

    /**
    *   @brief
    */
    void addWeapon(ActorWeapon* w);

    /**
    *   @brief
    */
    void removeWeapon(ActorWeapon* w);

    /**
    *   @brief
    */
    void addPowerup(Pickable* p);

    /**
    *   @brief
    */
    void removePowerup(Pickable* p);

    /**
    *   @brief
    */
    void setEquipedWeapon(ActorWeapon* weapon);

    /**
    *   @brief
    */
    ActorWeapon* getEquipedWeapon() {  return mEquipedWeapon;  }

    /**
    *   @brief
    */
    void indexContents();

signals:
    void equipedWeaponChanged();

public slots:

protected:
    ActorWeapon*              mEquipedWeapon = nullptr;
    std::list<ActorWeapon*>   mWeapons;
    std::list<Pickable*>      mPowerups;
    //later implementation.
    //std::list<Weapon*>   mWeaponsAvailable;
};

Q_DECLARE_METATYPE(ActorInventory*)

/**
*   @brief Simple struct containing the information for representing
*   a Pickable item, which will reference by an id to the specific
*   powerup or upgrade. Which will be contained in the parent Actor
*   type as a child if the referenceTypeId is 0 (zero).
*
*   @note The Pickable.available property is used to check wheter the actor can pick up
*   this Pickable. If the Pickable has a logic that might trigger its availability.
*   The Pickable.picked is used to know wheter its been picked up by an actor.
*   The Pickable.name name for the pickable.
*   The Pickable.type identifies what type of pickable it is.
*   The Pickable.restriction is used on logics and power ups.
*/
class Pickable: public QObject{
    Q_OBJECT
public:
    explicit Pickable(QObject* parent = nullptr){}

    enum Type:unsigned int{
        NONE,
        POWER_UP,
        BODY_ENHANCEMENT,
        WEAPON,
        WEAPON_ENHANCEMENT
    };

    virtual void onPickedByActor(Actor* pickedBy){
        Q_UNUSED(pickedBy)
    }

    enum Restriction:unsigned int{
        TIME = 1,
        USE_COUNT
    };


    int id;
    int referencedType;
    bool available;
    bool picked;
    QString name;
    Type type = NONE;
    Restriction restriction;
    Actor* parent = nullptr;
};

Q_DECLARE_METATYPE(Pickable*)




#endif // ACTORINVENTORY_H
