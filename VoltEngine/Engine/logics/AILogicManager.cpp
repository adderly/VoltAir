#include "AILogicManager.h"
#include "AILogic.h"
#include "Util.h"
#include "QMutex"

AILogicManager::AILogicManager(QObject* parent):
        Logic(parent),mCurrentLogic("")
{

}

AILogicManager::~AILogicManager()
{

}

void AILogicManager::setCurrentLogic(const QString& d)
{
    qDebug() << " setCurrentLogic = " << d;
    mCurrentLogic = d;
    mQueuedChange = true;
    setLogic(mCurrentLogic);
    emit currentLogicChanged();
}

void AILogicManager::addLogicRef(AILogic *logic)
{
    if(logic)
    {
        if(!m_logics.contains(logic->getName())){
            m_logics.insert(logic->getName(), logic);
        }else{
            qDebug() << " Trying to add existent LogicRef = " << logic->getName();
        }
    }
}

void AILogicManager::addLogicRefs(QList<AILogic*>& logics)
{
    for(AILogic* logic:logics){
        addLogicRef(logic);
    }
}


void AILogicManager::setLogic(const QString& name)
{
    if(m_logic_current && m_logic_current->getName() == name){
        qDebug()  << "Trying to change the logic for the same name : " << name;
        mQueuedChange = false;
        return;
    }
    if(m_logics.contains(name)){
        m_logic_current = m_logics[name];
    }

    if(!m_logic_current){
        qDebug()  << " Changed to a unexistent logic : " << name;
    }
    mQueuedChange = false;
}

void AILogicManager::init()
{
   Logic::init();
   if(!mActor)
       return;

   QList<AILogic*> ailogics =  mActor->findChildren<AILogic*>();
   qDebug() << "Logics found = " << ailogics.size();

   for(AILogic* ai_logic:ailogics)
   {
       if(!Util::nullOrEmpty(ai_logic->getName())){
           m_logics.insert(ai_logic->getName(),ai_logic);
       }else{
           qDebug() << " UNAMED AILogic found in Actor. " ;
       }

       if(ai_logic->currentAI()){
           setCurrentLogic(ai_logic->getName());
       }
   }

   if(!mCurrentLogic.isEmpty()){
       m_logic_current = m_logics[mCurrentLogic];
   }
}

void AILogicManager::update()
{
    if(!mQueuedChange && m_logic_current){

        switch (m_logic_current->AIState()) {
            case AILogic::ENTER:
                m_logic_current->onEnter();
                break;
            case AILogic::UPDATE:
                m_logic_current->update();
                break;
            case AILogic::EXIT:
//                m_logic_current->onExit();
                exitAIState(m_logic_current);
                break;
            case AILogic::INACTIVE:
                break;
            default:
                break;
        }
    }


}

void AILogicManager::enterAILogic(AILogic* logic)
{
    logic->onEnter();
}

void AILogicManager::exitAIState(AILogic* logic)
{
    logic->onExit();
    QString exit_to = logic->exitTo();

    //
    if(logic->preventStatelessExit() && exit_to.isEmpty()){
        return;
    }
    this->setCurrentLogic(exit_to);
}

