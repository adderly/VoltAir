#ifndef AISTATEINTERFACE
#define AISTATEINTERFACE

class AIStateBase{
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
};

#endif // AISTATEINTERFACE

