#include "ActorInventory.h"
#include "GameNodes/inventory/ActorWeapon.h"
#include <algorithm>
#include <QDebug>
#include "Actor.h"

ActorInventory::ActorInventory(QObject *parent) : QObject(parent)
{

}

ActorInventory::~ActorInventory()
{

}


void ActorInventory::addWeapon(ActorWeapon* w){
    mWeapons.emplace_back(w);
}

void ActorInventory::removeWeapon(ActorWeapon* w){
    mWeapons.remove(w);
}
void ActorInventory::addPowerup(Pickable* p){
    mPowerups.emplace_back(p);
}

void ActorInventory::removePowerup(Pickable* p){
    mPowerups.remove(p);
}

void ActorInventory::setEquipedWeapon(ActorWeapon* weapon){
    mEquipedWeapon = weapon;
    emit equipedWeaponChanged();
}

void ActorInventory::indexContents()
{
    QList<ActorWeapon*> weapons = this->findChildren<ActorWeapon*>();
    QList<ActorWeapon*>::iterator b = weapons.begin();
    Actor* owner = qobject_cast<Actor*>(parent());

    for(;b!= weapons.end();b++){
        (*b)->setOwner(owner);
        mWeapons.push_back(*b);
    }

    QVariant default_weapon = this->property("default_weapon");
    ActorWeapon *dweapon = default_weapon.value<ActorWeapon*>();

    if(dweapon){
        mEquipedWeapon = dweapon;
    }

    qDebug() << " Weapons found  = " << weapons.size();
    qDebug() << " Default Weapon = " << default_weapon;
    qDebug() << (dweapon == nullptr ? " Null pointer ":" Not null default weapon");
}

