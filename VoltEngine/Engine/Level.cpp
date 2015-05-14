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

#include <cassert>
#include "Camera.h"
#include "Engine.h"
#include "Level.h"
#include "Body.h"
#include "graphics/ImageRenderer.h"
#include "Logic.h"
#include "graphics/ParticleRendererItem.h"
#include "GameNodes/logics/PickupLogic.h"
#include "utils/Constants.h"
#include "utils/Util.h"
#include "utils/Box2dUtil.h"

const float Level::PARTICLE_RENDERER_LAYER_ZBIAS = 1.0E-6f;

Level::Level() {
    // Set ourselves as the current Level object in Engine, so that other QObjects that get
    // instantiated by the QML engine have access to this object, and the b2World.
    Engine::getInstance()->setCurrentLevel(this);

    // Create our Box2D world.
    mWorld.reset(new b2World(b2Vec2(mGravity.x(), mGravity.y())));
}

Level::~Level() {
    // We need to delete the b2World after deleting our children, which happens in our base class'
    // destructor. Queue the destruction in Engine.
    Engine::getInstance()->queueWorldForDeletion(mWorld.release());
    Engine::getInstance()->invalidateSceneGraphObjectCaches();
}

void Level::setName(const QString& value) {
    mName = value;
    emit nameChanged();
}

void Level::setFileName(const QString& value) {
    mFileName = value;
    emit fileNameChanged();
}

void Level::setJsonData(const QString &value){
    mJsonData = value;
    emit jsonDataChanged();
}

void Level::setJsonFilePath(const QString &value){
    mJsonFilePath = value;
    emit jsonFilePathChanged();
}

void Level::setGravity(const QPointF& value) {
    if (value == mGravity) {
        return;
    }
    mGravity = value;
    mWorld->SetGravity(b2Vec2(mGravity.x(), mGravity.y()));
    emit gravityChanged();
}

void Level::setParallaxOrigin(const QPointF& value) {
    if (value == mParallaxOrigin) {
        return;
    }
    mParallaxOrigin = value;

    // Copy it to our camera.
    Engine::getInstance()->getCamera()->setParallaxOrigin(value);

    emit parallaxOriginChanged();
}

void Level::setParallaxStrength(const QPointF& value) {
    if (value == mParallaxStrength) {
        return;
    }
    mParallaxStrength = value;
    emit parallaxStrengthChanged();
}

void Level::setParticleLayer(int value) {
    mParticleLayer = value;
    if (mParticleRendererItem) {
        updateParticleRendererItemZ();
    }
    emit particleLayerChanged();
}

void Level::setParticleAlpha(float value) {
    mParticleAlpha = value;
    emit particleAlphaChanged();
}

void Level::setParticleLayers(const QList<ParticleLayer*>& value) {
    mParticleLayers = value;
    emit particleLayersChanged();
}

QQmlListProperty<ParticleLayer> Level::getParticleLayerListProperty() {
    return Util::QQmlListPropertyOnQList<
            Level,
            ParticleLayer,
            QList<ParticleLayer*>,
            &Level::mParticleLayers,
            &Level::particleLayersChanged>::createList(this);
}

void Level::setCompletionThreshold(int value) {
    mCompletionThreshold = value;
    emit completionThresholdChanged();
}

void Level::setLevelType(LevelType &type){
    mLevelType = type;
    emit levelTypeChanged(mLevelType);
}

void Level::setMouseToInputArea(bool value){
    mMouseToInputArea = value;
    emit mouseToInputAreaChanged();
}

void Level::managedLevel()
{
    // Index the actors in the scene graph
    for (Actor* actor : findChildren<Actor*>())
    {
        mActors[actor->getActorType()].insert(actor);

        Body* aBody = actor->getBody();
        if(aBody && aBody->manualInitialization()){
            aBody->initBody();
//            aBody->updateBeforePhysics();
            Json::Value val  = mBox2dJson.b2j(aBody->getBody());
            qDebug() << "Manual Inited Body = " << QByteArray(val.toStyledString().c_str());
        }
        // For now, we define the teleporter to be an actor with the activationThreshold property.
        QVariant variant = actor->property("activationThreshold");
        if (variant.isValid()) {
            if (mPortal) {
                qDebug() << "Warning: Level has more than one teleporter.";
            }
            mPortal = actor;
        }
    }
}

void Level::rubeB2DLevel(b2dJson& mBox2dJson)
{
    if(mJsonFilePath.isEmpty()){
        qDebug() << " mJsonFilePath is empty ";
       return;
    }
    QString jsonfile = Util::getPathToAsset("assets/%1").arg(mJsonFilePath);
    if(!Util::fileExists(jsonfile)){
        qDebug() << " Level: "  << jsonfile << " does not exists. ";
        return;
    }

    QString filepath = Util::getPathToAsset("assets/levels/Colliseum.json");
    std::string worldJson = Util::readFileAsStdString(filepath);

    std::string errorstring;
    b2World* w = mBox2dJson.readFromString(worldJson,errorstring,mWorld.get());

    if(w){

        //custom properties to be read
        int useWorldGravity = mBox2dJson.getCustomInt(w,"use_world_gravity",0);
        float grav_x ;
        float grav_y ;
        if(useWorldGravity){
            grav_x = mBox2dJson.getCustomFloat(w,"world_gravity_x",0);
            grav_y = mBox2dJson.getCustomFloat(w,"world_gravity_y",0);

             w->SetGravity(b2Vec2(grav_x,grav_y));
        }

        qDebug() << " Reading jsonfile Complete!  \n\t ";
        Body* b = nullptr;
        b2Body* body = mWorld->GetBodyList();
        while(body){
            b = Body::BodyFromb2Body(body);
            if(b) b->setParent(this);
            body = body->GetNext();
        }

        //check if there is a camera body
        //used for camera bounds. (Square body)
        b2Body* cam_body = mBox2dJson.getBodyByName("camera");
        if(cam_body){
            b2Fixture* cam_fixture = cam_body->GetFixtureList();
            if(cam_fixture){
                QRectF bbox = Box2dUtil::toQRectF(cam_fixture->GetAABB(0));
                this->setCameraBoundary(bbox);
                cam_body->SetActive(false);
#ifdef D_PARSE
                qDebug() << "Bounding box "<< bbox;
#endif
            }
            qDebug() << "camera found!";
        }


       //check for images associated with the body.
       int imageCount = 0;
       body = nullptr;
       b = nullptr;
       std::vector<b2dJsonImage*> world_images;
       imageCount = mBox2dJson.getAllImages(world_images);
       Actor* actor = nullptr;
       ImageRenderer* img_renderer = nullptr;
       QString image_path;
       for(b2dJsonImage* img: world_images){
           if((body = img->body)){
               b = static_cast<Body*>(img->body->GetUserData());
               if(b){
                   image_path = Util::getPathToLevel(QString(img->file.c_str()));
                   actor = new Actor(b);
                   actor->setPosition(b->getPosition());
                   actor->setParent(this);
                   actor->setParentItem(this);
                   img_renderer = new ImageRenderer(actor);
                   img_renderer->setSource(image_path);
                   img_renderer->setSizeScale(img->scale);
                   img_renderer->setVisible(true);
                   img_renderer->setCacheRenderParams(body->GetType() == Body::StaticBody);

                   QRectF c;
                   c.setTopLeft(Box2dUtil::toQPointF(img->corners[0]));
                   c.setTopRight(Box2dUtil::toQPointF(img->corners[1]));
                   c.setBottomRight(Box2dUtil::toQPointF(img->corners[2]));
                   c.setBottomLeft(Box2dUtil::toQPointF(img->corners[3]));
                   img_renderer->setCustomLocalBox(c);

#ifdef D_PARSE
                   qDebug() << image_path << " box2d " << Box2dUtil::toQPointF(img->corners[0]);
                   qDebug() << image_path << " box2d " << Box2dUtil::toQPointF(img->corners[1]);
                   qDebug() << image_path << " box2d " << Box2dUtil::toQPointF(img->corners[2]);
                   qDebug() << image_path << " box2d " << Box2dUtil::toQPointF(img->corners[3]);
                   qDebug() << image_path << " render localbox " << c;
                   //TODO: fixe the scale ratio, to match box2d.
                   b2AABB aabb = img->getAABB();
                   qDebug() <<"Image aabb = " << Box2dUtil::toQRectF(aabb);
                  // img_renderer->setPosition(QPointF(img->center.x,img->center.y ));


                   qDebug() << "Image pos = " <<img_renderer->position();
#endif
               }
           }
       }
    }else{
        qDebug() << "Error Reading jsonfile: \n\t " << errorstring.c_str();
    }
    w = nullptr;
}

void Level::componentComplete() {
    QQuickItem::componentComplete();

    // Create a ParticleRendererItem. Note that we do it in componentComplete() as
    // we want it to be the absolute last item in the level.
    ParticleRendererItem* particleRendererItem = new ParticleRendererItem();
    particleRendererItem->setParent(this);
    particleRendererItem->setParentItem(this);
    mParticleRendererItem = particleRendererItem;
    updateParticleRendererItemZ();

    switch(mLevelType){
        case RUBE_BOX2D_LEVEL:
            rubeB2DLevel(mBox2dJson);
            break;
        default:break;
    }

    //DEFAULT CAMERA
    managedLevel();

//    std::string worldstr  = mBox2dJson.writeToString(mWorld.get());
//    qDebug() << "World STRING : " << QByteArray(worldstr.c_str());

    // Determine the number of pickups that exist in the static level
    mStartingNumPickups = findChildren<PickupLogic*>().size();

    if (mCompletionThreshold < 0) {
        emit completionThresholdChanged();
    }

    // Check invariant that the activationThreshold is no larger than the completion threshold
    if (mPortal) {
        Q_ASSERT(getCompletionThreshold() >= mPortal->property("activationThreshold").toInt());
    }

    // TODO: Add sanity check that completion threshold can be reached.
}

void Level::updateParticleRendererItemZ() {
    // Set the particle renderer to render just beneath the layer specified.
    // Since the Layer property of Actors is an integer, this is valid.
    mParticleRendererItem->setZ(((float) mParticleLayer) - PARTICLE_RENDERER_LAYER_ZBIAS);
}

void Level::setCameraBoundary(const QRectF& value) {
    mCameraBoundary = value;
    emit cameraBoundaryChanged();
}

void Level::setFillCamera(bool value) {
    mFillCamera = value;
    emit fillCameraChanged();
}

void Level::addActor(Actor* actor) {
    // Only add the actor to the Qt scene graph if it does not already exist in it
    if (mActors[actor->getActorType()].contains(actor)) {
        return;
    }

    // Recursive add all the child actors
    mActors[actor->getActorType()].insert(actor);
    for (Actor* childActor : actor->findChildren<Actor*>()) {
        // A child cannot have more than one parent in a valid scene graph
        assert(!mActors[childActor->getActorType()].contains(childActor));
        mActors[childActor->getActorType()].insert(childActor);

        Body* aBody = childActor->getBody();
        if(aBody && aBody->manualInitialization()){
            aBody->initBody();
        }
    }

    // Insert the actor into the Qt scene graph
    actor->setParent(this);
    actor->setParentItem(this);

    // Perform any initialization of the new scene graph components
    // TODO: Refactor into queue inside of engine to avoid possible double initialization.
    for (Logic* logic : actor->findChildren<Logic*>()) {
        logic->init();
    }

    // Invalidate the Engine's cached view of the scene graph
    Engine::getInstance()->invalidateSceneGraphObjectCaches();
}

const QSet<Actor*>& Level::getActors(Actor::ActorType type) const {
    // Cannot use "value()" which returns by value, and we do not want the list copied
    const auto& it = mActors.find(type);
    if (it != mActors.end()) {
        return it.value();
    } else {
        return Constants::EMPTY_ACTOR_SET;
    }
}


