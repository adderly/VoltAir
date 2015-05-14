/*
 * Copyright (C) 2014 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QList>
#include <QMap>

#include "Actor.h"
#include "Body.h"
#include "Engine.h"
#include "logics/AILogic.h"
#include "logics/AILogicManager.h"
#include "utils/Util.h"
#include "ActorProperties.h"
#include "ActorInventory.h"

Actor::Actor(Body* body):mBody(body) {
    connect(this, &QQuickItem::childrenChanged,
            this, &Actor::indexChildren);
    connect(this, &QQuickItem::xChanged,
            this, &Actor::invalidateBodyTransform,
            Qt::DirectConnection); // DirectConnection = force execution in event's source thread
    connect(this, &QQuickItem::yChanged,
            this, &Actor::invalidateBodyTransform,
            Qt::DirectConnection);
    connect(this, &QQuickItem::rotationChanged,
            this, &Actor::invalidateBodyTransform,
            Qt::DirectConnection);
    if(mBody){
        mBody->setParent(this);
    }
    mLogicManager = new AILogicManager(this);
}

void Actor::componentComplete() {
    QQuickItem::componentComplete();
    // Sometimes during load the signal childrenChanged is not called at the right time.
    // Call it now to ensure we find our children.
    indexChildren();
}

void Actor::invalidateBodyTransform() {
    if (mBody && Engine::getInstance()->getPhase() != Engine::AFTER_PHYSICS_PHASE) {
        mBody->invalidateTransform();
    }
}

void Actor::setActorType(ActorType value) {
    mActorType = value;
    emit actorTypeChanged();
}

Body* Actor::getBody() const {
    return mBody;
}

ActorProperties* Actor::getProperties(){
    return mProperties;
}

void Actor::setInventory(ActorInventory* value){
    mInventory = value;
}

QObject* Actor::findChildByTypeName(const QString& typeName) {
    // Convert to a cstring and look up type.
    std::string typeNameStr(typeName.toStdString());
    int typeId = QMetaType::type(typeNameStr.c_str());
    if (typeId == QMetaType::UnknownType) {
        return nullptr;
    }
    const QMetaObject* metadata = QMetaType::metaObjectForType(typeId);

    return findChildByType(this, *metadata);
}

// TODO: See if this function can be implemented more efficiently
QObject* Actor::findChildByType(QObject* item, const QMetaObject& metadata) {
    if (metadata.cast(item)) {
        return item;
    }

    for (QObject* child : item->children()) {
        QObject* returnItem = findChildByType(child, metadata);
        if (returnItem) {
            return returnItem;
        }
    }

    return nullptr;
}

void Actor::indexChildren() {
    mBody = findChild<Body*>();

    qDebug() << " Component complete ";
    mProperties = findChild<ActorProperties*>();

   qDebug() << (mProperties ? "ActorProperties yay!":" No ActorProperties -__-");

   mInventory = findChild<ActorInventory*>();
    if(mInventory){
        mInventory->indexContents();
    }
//    qDebug() << (mInventory ? "Inventory yay!":" No inventory -__-");
}
