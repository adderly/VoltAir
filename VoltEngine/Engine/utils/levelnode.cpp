#include "levelnode.h"

LevelNode::LevelNode(QObject *parent)
{

}

void LevelNode::setType(LevelNode::Type value){
    mType = value;
}

void LevelNode::setTargetName(QString& value){
    mTargetName = value;
}

void LevelNode::setTargetBody(QString& value){
    mTargetBody = value;
}

