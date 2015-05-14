#ifndef ACTORINVENTORY_H
#define ACTORINVENTORY_H

#include <QObject>

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


/**
*   @brief
*/
class Pickable{
public:
    enum Type:unsigned int{
        NONE,
        POWER_UP,
        BODY_ENHANCEMENT,
        WEAPON,
        WEAPON_ENHANCEMENT
    };

    enum Restriction:unsigned int{
        TIME,
        USE_COUNT
    };

    int powerValue;
    Type type;
    Restriction restriction;
    QString name;
};




Q_DECLARE_METATYPE(ActorInventory*)
#endif // ACTORINVENTORY_H
