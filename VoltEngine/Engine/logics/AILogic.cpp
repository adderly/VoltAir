#include "AILogic.h"
#include "utils/Util.h"

AILogic::AILogic(QObject* parent):Logic(parent){
    mAIManaged = true;
}

void AILogic::setAIState(AILogic::AI value)
{
    mAIState = value;
    emit aiStateChanged();
}

void AILogic::setExitTo(QString& state){
    m_exit_to = state;
    emit exitToChanged();
}

void AILogic::setCurrentAI(bool value){
    mCurrentAI = value;
    emit currentAIChanged();
}

void AILogic::setPreventStatelessExit(bool value){
    mPreventStatelessExit = value;
}

void AILogic::init(){
    // Find our Actor.
    mActor = Util::findParentOfType<Actor>(this);
    mAIStateOnExit = mAIState;
}

void AILogic::onEnter(){
    this->enter();
}

void AILogic::onExit(){
    this->exit();
}


void AILogic::enter(){
    mAIState = AILogic::UPDATE;
}

void AILogic::exit(){
    mAIState = INACTIVE;
}

void AILogic::resetLogic()
{
    mAIState = mAIStateOnExit;
}

