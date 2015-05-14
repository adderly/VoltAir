#ifndef INPUTLOGIC_H
#define INPUTLOGIC_H

#include<Engine/Logic.h>

/**
*   This logic is supposed to ease the way player actor gets its input.
*/
class InputLogic : public Logic
{
    Q_OBJECT
public:
    InputLogic(QObject *parent = nullptr);
    ~InputLogic();

    virtual void init() override;

    virtual void update() override;

};
Q_DECLARE_METATYPE(InputLogic*)

#endif // INPUTLOGIC_H
