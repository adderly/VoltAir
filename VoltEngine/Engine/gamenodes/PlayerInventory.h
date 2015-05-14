#ifndef PLAYERINVENTORY_H
#define PLAYERINVENTORY_H

#include "ActorInventory.h"


class PlayerInventory : public ActorInventory
{
    Q_OBJECT
public:
    PlayerInventory(QObject* parent = nullptr);
    ~PlayerInventory();
};

#endif // PLAYERINVENTORY_H
