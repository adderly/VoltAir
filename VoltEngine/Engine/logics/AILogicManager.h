#ifndef AILOGICMANAGER_H
#define AILOGICMANAGER_H

#include <QHash>
#include <QObject>
#include <Logic.h>

class AILogic;

/**
* @brief This will simulate behaviors as a FSM.
* @note Implementing this custom manager allow us to control specific logic behaviors
* like, if some actors needs to stop or being standing by while waiting for queue. Instead
* of updating all logics and controlling this from inside the logic itself. We can do it here.
*
* TODO: Add messaging capabilities for actors.
*       Example: If we enter some trigger area, the trigger area would like to "broadcast" a
*                message to nearby actors. The actor could have the recieve method, but should
*                be handled here cause we manage.
*/
class AILogicManager: public Logic
{
    Q_OBJECT

    Q_PROPERTY(QString currentLogic READ getCurrentLogic WRITE setCurrentLogic NOTIFY currentLogicChanged)
public:
    explicit AILogicManager(QObject* parent = nullptr);
    ~AILogicManager();

    /**
    *   @brief Sets the #currentLogic property.
    *   @note This is somewhat queued, and then changed. It's done like this to have a FSM
    *   structure.
    */
    void setCurrentLogic(const QString& d);

    /**
    *   @brief This actually sets the logic for execution.
    *   @note  The difference between this and  setCurrentLogic is that the former one queues it,
    *   and this put it into motion.
    */
    void setLogic(const QString& name);

    /**
    *   @brief Returns the #currentLogic.
    */
    QString getCurrentLogic() { return mCurrentLogic; }

    /**
    *   @brief Adds an AILogic to the container #m_logics.
    */
    void addLogicRef(AILogic* logic);

    /**
    *   @brief Adds the logics to the container #m_logics.
    */
    void addLogicRefs(QList<AILogic*>& l);

    /**
    *   @brief Override to filter the AILogics and set everything up.
    */
    void init() override;

    /**
    *   @brief Implementation of update for the "FSM".
    */
    virtual void update() override;

    /**
    *   @brief Execute enter on logic.
    */
    void enterAILogic(AILogic* logic);

    /**
    *   @brief Execute exit on logic.
    */
    void exitAIState(AILogic* logic);


    friend class AILogic;

signals:
    void currentLogicChanged();

private:

    /**
    *   @brief Sets the param to be queued as the current logic.
    */
    void setLogic(AILogic* logic);


    bool mQueuedChange = false;
    QString mCurrentLogic;

    AILogic* m_logic_current = nullptr;
    QHash<QString,AILogic*> m_logics;
};

Q_DECLARE_METATYPE(AILogicManager*)

#endif // AILOGICMANAGER_H
