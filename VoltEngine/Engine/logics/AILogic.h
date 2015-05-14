#ifndef AILOGIC_H
#define AILOGIC_H
#include <QObject>
#include <Engine/Logic.h>
#include <logics/AIStateInterface.h>
#include "AILogicManager.h"

/**
 * @brief The AILogic class will be used to control an entity.
 * @note Basically nodes for A.I. stuff. This would be used by the AILogicMAanager
 * for handling them as a Finite Time Maching states. That explains the onExit and
 * onEnter methods.
 * These should be executed on subclasses of AILogic, for providing that functionanilty.
 */
class AILogic : public Logic, AIStateBase
{
    Q_OBJECT

    Q_PROPERTY(AI AIState READ AIState WRITE setAIState NOTIFY aiStateChanged)
    Q_PROPERTY(QString exitTo READ exitTo WRITE setExitTo NOTIFY exitToChanged)
    Q_PROPERTY(bool currentAI READ currentAI WRITE setCurrentAI NOTIFY currentAIChanged)
    Q_PROPERTY(bool preventStatelessExit READ preventStatelessExit WRITE setPreventStatelessExit NOTIFY preventStatelessExitChanged)
    Q_ENUMS(AI)
public:
    enum AI:unsigned int{
        ENTER = 0,
        UPDATE,
        EXIT,
        INACTIVE //used after exiting
    };

    explicit AILogic(QObject* parent = nullptr);

    /**
    *   @brief Sets the #AIState.
    */
    void setAIState(AI value);

    /**
    *   @brief Gets the AIState.
    */
    AI AIState() {  return mAIState;  }

    /**
    *   @brief Sets the manager to this one.
    *   @note Why, i created this? who knows! I'll let the NSA find out.
    */
    void setLogicManager(AILogicManager* mgr){
        mAIManager = mgr;
    }
    /**
    *   @brief Returns the logic this would call up when exiting.
    */
    QString exitTo() {  return m_exit_to; }

    /**
    *   @brief Sets the exit state for this state.
    */
    void setExitTo(QString& state);

    /**
    *   @brief Returns whether this is the current logic to be executed by the AIManager.
    *   @note  This is meant for initialialization, means this lets the AIManager knows
    *   this is the first logic to execute.
    */
    bool currentAI() {  return mCurrentAI;  }

    /**
    *   @brief Sets whether this is the init logic to execute.
    */
    void setCurrentAI(bool value);

    /**
    *   @brief
    */
    bool preventStatelessExit() {  return mPreventStatelessExit;  }

    /**
    *   @brief Sets the #preventStatelessExit.
    */
    void setPreventStatelessExit(bool value);

    /**
    *   @brief Resets the logic to the defaults.
    *   @note Used when onExit is executed.
    */
    void resetLogic();

    /**
    *   @brief Override Logic
    *   TODO: This should be only override on the AILogic subclasses, little unneeded overhead.
    *   AILogicSubclass->init()->AILogic::init()->Logic::init()..... WTF, so stupid.
    */
    virtual void init() override;

    /**
    *   @brief Subclass implementation.
    *   @note when subclassing make sure either you call base implementation
    *   or implement what is needed for the AILogic to have it normal behavior.
    *   @note As found before there is some threading issues if these logics call
    *   method in its manager, for that reason. All the logic involving the
    *   manager have been moved there. Here you can execute only code regarding this object.
    */
    virtual void onEnter();

    /**
    *   @brief Subclass implementation.
    *   @note when subclassing make sure either you call base implementation
    *   or implement what is needed for the AILogic to have it normal behavior.
    *   @note As found before there is some threading issues if these logics call
    *   method in its manager, for that reason. All the logic involving the
    *   manager have been moved there. Here you can execute only code regarding this object.
    */
    virtual void onExit();

signals:
    void exitToChanged();
    void currentAIChanged();
    void aiStateChanged();
    void preventStatelessExitChanged();

protected:
     void enter();
     void exit();

    ///this is the default, in case don't want enter, just set it to UPDATE.
    AI mAIState = ENTER;
    ///This is used to controll whether you can exit when the #m_exit_to is empty.
    bool mPreventStatelessExit = false;

private:
    ///this holds the value mAIState was initialized with in on exit this will be the state.
    AI mAIStateOnExit;
    /// Identifier for the manager whether this is the "default or initial" logic/state.
    bool mCurrentAI = false;
    /// The state that would be called when this one exits.
    QString m_exit_to;
    /// I Am your father, i mean manager.
    AILogicManager* mAIManager = nullptr;
};


Q_DECLARE_METATYPE(AILogic*)

#endif // AILOGIC_H
