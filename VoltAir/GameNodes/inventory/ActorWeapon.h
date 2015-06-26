#ifndef ACTORWEAPON_H
#define ACTORWEAPON_H

#include <Engine/ActorInventory.h>
#include <QPointF>

class Actor;

/**
*   @brief Subclass of the weapon that the Actor will be handling around.
*/
class ActorWeapon: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(float damageFactor READ damageFactor WRITE setDamageFactor NOTIFY damageFactorChanged)
    Q_PROPERTY(int reloadSpeed READ reloadSpeed WRITE setReloadSpeed NOTIFY reloadSpeedChanged)
    Q_PROPERTY(int magSize READ magSize WRITE setMagSize NOTIFY magSizeChanged)
    Q_PROPERTY(int ammoAmount READ ammoAmount WRITE setAmmoAmount NOTIFY ammoAmountChanged)
    Q_PROPERTY(QString  name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(float attackDistance READ attackDistance WRITE setAttackDistance NOTIFY attackDistanceChanged)
    Q_PROPERTY(bool autoReload READ autoReload WRITE setAutoReload NOTIFY autoReloadChanged)
public:
    ActorWeapon(QObject* parent = nullptr);

    enum WeaponType:int{
        GUN, //rockets bow
        BOMB,
        KNIFE,
        CUSTOM
    };

    //priority
    enum Type{
        PRIMARY,
        SECUNDARY
    };

    enum BulletType:unsigned int{
        NORMAL,
        ARROW,
        SPECIAL
    };

    void setEnabled(bool value){
        mEnabled = value;
        emit enabledChanged();
    }

    bool enabled() {  return mEnabled;  }

    void setDamageFactor(float value){
        mDamageFactor = value;
        emit damageFactorChanged();
    }

    float damageFactor() {  return mDamageFactor;  }

    void setReloadSpeed(int value){
        mReloadSpeed = value;
        emit reloadSpeedChanged();
    }

    int reloadSpeed() {  return mReloadSpeed;  }

    void setMagSize(int value) {
        mMagSize = value;
        emit magSizeChanged();
    }

    int magSize() {  return mMagSize;  }

    void setAmmoAmount(int value) {
        mAmmoAmount =  value;
        emit ammoAmountChanged();
    }

    int ammoAmount() {  return mAmmoAmount;  }

    void setName(QString value) {
        mName = value;
        emit nameChanged();
    }

    QString name() {  return mName;  }

    /**
    *   @brief
    */
    void setAttackDistance(float value);

    /**
    *   @brief
    */
    float attackDistance() {  return mAttackDistance;  }

    /**
    *   @brief
    */
    void setAutoReload(bool value);

    /**
    *   @brief
    */
    bool autoReload() {  return mAutoreload;  }

    /**
    *   @brief Set the owner of the weapon.
    */
    void setOwner(Actor* value);

    /**
    *   @brief Return the actor holding the weapon.
    */
    Actor* owner() {  return mOwner;  }

    /**
    *   @brief The attack function for the weapon.
    *   @note By default this method calls a javascript function "attackFunc"
    *   if exists. This is mainly for fast weapon prototyping.
    */
    virtual void attack();

    /**
    *   @brief
    */
    virtual void reload(){}

    /**
    *   @brief This is used to handle some logic equiping the weapon.
    */
    virtual void onEquiped(){}

    /**
    *   @brief Method for creating bullet bodies.
    */
    Actor* createBullet(BulletType type);

signals:
    void enabledChanged();
    void damageFactorChanged();
    void reloadSpeedChanged();
    void magSizeChanged();
    void ammoAmountChanged();
    void nameChanged();
    void attackDistanceChanged();
    void autoReloadChanged();

protected:

    bool  mEnabled;
    float mDamageFactor = 0.0f;
    int   mReloadSpeed;
    int   mMagSize;
    int   mAmmoAmount;
    QString mName;

    float mAttackDistance = 0.0f;
    bool  mAutoreload = true;
    Actor* mOwner = nullptr;
};


/**
*   @brief Some bodies will have a ContactManifold to ease the
*   identification process, for the contacts between bodies.
*/
class ContactManifold {
public:
    enum Type: unsigned int{
        NONE,
        WEAPON,
        PICKABLE,
        CUSTOM
    };
    int   customId;
    float damageFactor = 0.0f;
    Type type = NONE;
    QObject* contactType = nullptr;
};

Q_DECLARE_METATYPE(ActorWeapon*)

#endif // ACTORWEAPON_H
