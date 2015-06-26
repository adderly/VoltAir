#ifndef LEVELNODE_H
#define LEVELNODE_H

#include <QObject>
#include <QVariant>
#include <Actor.h>

/**
*   @brief Object used as definition for instantiating object
*   based on the Level loaders (Mainly  RubeLoader).
*   @note This will serve as medium between the Qml level and the Json level.
*   Example:
*
*   LevelNode{
*        type:LevelNode.ACTOR_COMPONENT_W_BODY
*        targetName: "qml/game/actors/Enemy.qml"
*        targetBody:"body5"
*    }
*
*`  Actor{
*        x: 1
*        y: 1
*        LevelNode{
*            type:LevelNode.BODY_REFERENCE
*            targetName: ""
*            targetBody:"body7"
*        }
*    }
*
*/
class LevelNode: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType )
    /**
    *   @brief  Target name will be used to identify the property or target name
    *           depending in the Type of the node.
    *   @note   When Type == BODY_REFERENCE , targetname means the id of the body
    *           in the world.
    *           Type == BODY_PROPERTY_REFERENCE, target name means a property of the body.
    *           Type == ACTOR_COMPONENT_W_BODY, target name means the component url in the
    *           qml/actors directory.
    */
    Q_PROPERTY(QString targetName READ targetName WRITE setTargetName)

    /**
    *   @brief Used in case of actors, as the target name must aim to the actor qml file.
    */
    Q_PROPERTY(QString targetBody READ targetBody WRITE setTargetBody)

    Q_ENUMS(Type)
public:
    LevelNode(QObject* parent = nullptr);
    /**
    *   @brief BODY_REFERENCE = An exiting actor References a body from the world.
    *          BODY_HANDLER = Will execute a worker Logic, with the needed interface.
    *          BODY_PROPERTY_REFERENCE = An existing actor needs a property from a body.
    *          ACTOR_COMPONENT_W_BODY = An Actor will be instantiated from a .qml file and
    *           assign the body to it.
    */
    enum Type{
        BODY_REFERENCE = 0,
        BODY_HANDLER,
        BODY_PROPERTY_REFERENCE,
        ACTOR_COMPONENT_W_BODY
    };
    /**
    *   @brief Returns the type variable.
    */
    Type type() {  return mType;  }
    /**
    *   @brief Sets the type varaible.
    */
    void setType(Type value);
    /**
    *   @brief Returns the taget name variable.
    */
    QString targetName() {  return mTargetName;  }
    /**
    *   @brief Sets target name variable.
    */
    void setTargetName(QString& value);
    /**
    *   @brief
    */
    QString targetBody() {  return mTargetBody;  }
    /**
    *   @brief
    */
    void setTargetBody(QString& value);


signals:
//    void typeChanged();
//    void targetNameChanged();
//    void targetBodyChanged();
private:
    Type mType;
    QString mTargetName;
    QString mTargetBody;
    Actor* mActorParent = nullptr;
};
Q_DECLARE_METATYPE(LevelNode*)

#endif // LEVELNODE_H
