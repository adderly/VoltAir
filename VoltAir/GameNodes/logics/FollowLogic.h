#ifndef FOLLOWLOGIC_H
#define FOLLOWLOGIC_H
#include "Logic.h"


class FollowLogic: public Logic
{
    Q_OBJECT
public:
    FollowLogic(QObject* parent= nullptr);
    ~FollowLogic();

    virtual void init() override;
    virtual void update() override;

private:
    Body* mBody = nullptr;
};
Q_DECLARE_METATYPE(FollowLogic*)

#endif // FOLLOWLOGIC_H
