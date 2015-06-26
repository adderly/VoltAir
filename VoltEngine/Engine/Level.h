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

#ifndef LEVEL_H
#define LEVEL_H

#include <QMap>
#include <QQuickItem>
#include <QSet>
#include <limits>
#include <memory>
#include "Actor.h"

#ifdef BOX2D_LOADER
#include "b2dJson.h"
#include "b2dJsonImage.h"
#endif


class ParticleLayer;
class ParticleRendererItem;
class b2World;
class LevelNode;
/**
 * @ingroup Engine
 * @brief Representation of an in-game level.
 *
 * A Level is a discrete section of the game, each with a well defined start and end. Most Level%s
 * in VoltAir start at the Robot's spawn point and end when the Robot successfully enters an
 * activated Portal.
 *
 * Level%s are comprised of a collection of Actor%s (for example, the Robot). The data describing
 * the initial state of these Actor%s is loaded from data (QML) and instantiated into a tree
 * structure. Nodes are QQuickItem%s or QObject%s, and form part of the QML tree. The Level object
 * itself becomes the parent of all of these objects.
 *
 * Aside from containing Actor%s, the Level is responsible for maintaining some of the state that is
 * global to the Level, such as the intensity of gravity. The Level is also the owner of the physics
 * world (@c b2World).
 *
 * @note When loading from the Rube json format, there are two ways of handling actors:
 *      1 - Using LevelNode objects to specify level objects.
 *      2 - Using category mapping, which means that the object from the rube loader
 *      comes with a property defining which type of actor it represents. With this objects
 *      are automatically instantiated using the object.property mapped with the Actor it needs.
 *      It is done using a customProperty (actorType), in the bodies comming from
 *      The mapping is done in a mLevelActorMapping in the Level.
 *      Example:
 *      levelActorMapping:[
 *        {     "enemy":"qml/game/actors/Enemy.qml"      }
 *      ]
 *
 */
class Level : public QQuickItem {
    Q_OBJECT

    /**
     * @brief Rectangular region in world coordinates outside of which the Camera must not display.
     */
    Q_PROPERTY(QRectF cameraBoundary READ getCameraBoundary WRITE setCameraBoundary
            NOTIFY cameraBoundaryChanged)
    /**
     * @brief @c true if the Camera should be fixed to the #cameraBoundary, and always view the
     * extents of that region.
     *
     * If @c false, the Camera tries to follow the location of the Actor%s marked as
     * Actor::PlayerActor.
     * @see CameraMovementLogic
     */
    Q_PROPERTY(bool fillCamera READ shouldFillCamera WRITE setFillCamera NOTIFY fillCameraChanged)
    /**
     * @brief Name of this Level.
     */
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    /**
     * @brief JsonData of this Level.
     */
    Q_PROPERTY(QString jsonData READ getJsonData WRITE setJsonData NOTIFY jsonDataChanged)
    /**
     * @brief JsonFilePath of this Level.
     */
    Q_PROPERTY(QString jsonFilePath READ getJsonFilePath WRITE setJsonFilePath NOTIFY jsonFilePathChanged)
    // TODO: Remove this property when then QML Loader bug is fixed.
    /**
     * @brief Path to the file that this Level was loaded from.
     */
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)
    /**
     * @brief Strength of gravity.
     */
    Q_PROPERTY(QPointF gravity READ getGravity WRITE setGravity NOTIFY gravityChanged)
    /**
     * @brief Number of Actor%s with PickupLogic%s, representing the total number of objects that
     * can be collected in the Level (e.g. the number of Orb Actor%s in the Level).
     */
    Q_PROPERTY(int startingNumPickups READ getStartingNumPickups)
    /**
     * @brief Location of the parallax origin.
     * @see ParallaxTransformItem
     */
    Q_PROPERTY(QPointF parallaxOrigin READ getParallaxOrigin WRITE setParallaxOrigin
            NOTIFY parallaxOriginChanged)
    /**
     * @brief Amount of offset Graphic%s under a ParallaxTransformItem will receive.
     * @see ParallaxTransformItem
     */
    Q_PROPERTY(QPointF parallaxStrength READ getParallaxStrength WRITE setParallaxStrength
            NOTIFY parallaxStrengthChanged)
    /**
     * @brief Z-depth that the particles rendered by ParticleRendererItem should be at.
     *
     * This controls what Graphic%s they appear in front and behind.
     */
    Q_PROPERTY(int particleLayer READ getParticleLayer WRITE setParticleLayer
            NOTIFY particleLayerChanged)
    /**
     * @brief Overall transparency of the particles rendered by ParticleRendererItem, with @c 0.0f
     * being completely transparent, and @c 1.0f being maximally opaque.
     */
    Q_PROPERTY(float particleAlpha READ getParticleAlpha WRITE setParticleAlpha
            NOTIFY particleAlphaChanged)
    /**
     * @brief QML mutable list of ParticleLayer%s, each representing a physically independent layer
     * of LiquidFun particles.
     * @see ParticleLayer
     */
    Q_PROPERTY(QQmlListProperty<ParticleLayer> particleLayers READ getParticleLayerListProperty
            NOTIFY particleLayersChanged)
    /**
     * @brief Points threshold for which the level can be considered "complete".
     * @see Game
     */
    Q_PROPERTY(int completionThreshold READ getCompletionThreshold WRITE setCompletionThreshold
            NOTIFY completionThresholdChanged)
    /**
    * @brief
    */
    Q_PROPERTY(QVariantList registeredActors READ registeredActors WRITE setRegisteredActors
           NOTIFY registeredActorsChanged)
    /**
    *   @brief
    */
    Q_PROPERTY(QVariantList levelActorMapping READ levelActorMapping WRITE setLevelActorMapping )
    /**
     * @brief Helper property to find the Portal for this Level.
     */
    Q_PROPERTY(Actor* portal READ getPortal)
    /**
    * @brief LevelType Enum.
    */
    Q_ENUMS(LevelType)
    /**
    * @brief Holds the levelType.
    */
    Q_PROPERTY(LevelType levelType READ levelType WRITE setLevelType NOTIFY levelTypeChanged)
    /**
    * @brief Holds the mouseToInputArea.
    * Note: if some logic need the mouse, it can access it from the InputArea
    */
    Q_PROPERTY(bool mouseToInputArea READ mouseToInputArea WRITE setMouseToInputArea NOTIFY mouseToInputAreaChanged)

public:
    /**
     * @brief Construct a Level.
     */
    explicit Level();
    virtual ~Level();

    /**
     * @brief Returns #cameraBoundary.
     */
    QRectF getCameraBoundary() const { return mCameraBoundary; }
    /**
     * @brief Sets #cameraBoundary.
     * @param value QRectF to set #cameraBoundary to
     */
    void setCameraBoundary(const QRectF& value);
    /**
     * @brief Returns #fillCamera.
     */
    bool shouldFillCamera() const { return mFillCamera; }
    /**
     * @brief Sets #fillCamera.
     * @param value Boolean to set #fillCamera to
     */
    void setFillCamera(bool value);
    /**
     * @brief Returns #name.
     */
    const QString& getName() const { return mName; }
    /**
     * @brief Sets #name.
     * @param value QString to set #name to
     */
    void setName(const QString& value);
    /**
     * @brief Returns #fileName.
     */
    const QString& getFileName() const { return mFileName; }
    /**
     * @brief Sets #fileName.
     * @param value QString to set #fileName to
     */
    void setFileName(const QString& value);
    /**
     * @brief Sets #jsonData.
     * @param value QString to set #jsonData to
     */
    void setJsonData(const QString& value);
    /**
     * @brief Returns #jsonData.
     */
    const QString& getJsonData() const { return mJsonData; }
    /**
     * @brief Sets #sonFilePath.
     * @param value QString to set #jsonFilePath to
     * Note: the routes are based in the "assets/" folder.
     */
    void setJsonFilePath(const QString& value);
    /**
     * @brief Returns #jsonFilePath.
     */
    const QString& getJsonFilePath() const { return mJsonFilePath; }
    /**
     * @brief Returns #gravity.
     */
    QPointF getGravity() const { return mGravity; }
    /**
     * @brief Sets #name.
     * @param value QPointF to set #name to
     */
    void setGravity(const QPointF& value);
    /**
     * @brief Returns #startingNumPickups.
     */
    int getStartingNumPickups() const { return mStartingNumPickups; }
    /**
     * @brief Returns #parallaxOrigin.
     */
    QPointF getParallaxOrigin() const { return mParallaxOrigin; }
    /**
     * @brief Sets #parallaxOrigin.
     * @param value QPointF to set #parallaxOrigin to
     */
    void setParallaxOrigin(const QPointF& value);
    /**
     * @brief Returns #parallaxStrength.
     */
    QPointF getParallaxStrength() const { return mParallaxStrength; }
    /**
     * @brief Sets #parallaxStrength.
     * @param value QPointF to set #parallaxStrength to
     */
    void setParallaxStrength(const QPointF& value);
    /**
     * @brief Returns #particleLayer.
     */
    int getParticleLayer() const { return mParticleLayer; }
    /**
     * @brief Sets #particleLayer.
     * @param value Integer to set #particleLayer to
     */
    void setParticleLayer(int value);
    /**
     * @brief Returns #particleAlpha.
     */
    float getParticleAlpha() { return mParticleAlpha; }
    /**
     * @brief Sets #particleAlpha.
     * @param value Float to set #particleAlpha to
     */
    void setParticleAlpha(float value);
    /**
     * @brief Returns #particleLayers as a QList.
     */
    const QList<ParticleLayer*>& getParticleLayers() const { return mParticleLayers; }
    /**
     * @brief Sets #particleLayers from a QList.
     * @param value QList of ParticleLayer%s to set #particleLayers to
     */
    void setParticleLayers(const QList<ParticleLayer*>& value);
    /**
     * @brief Returns #particleLayers as a QML mutable list.
     */
    QQmlListProperty<ParticleLayer> getParticleLayerListProperty();
    /**
     * @brief Returns #completionThreshold.
     */
    int getCompletionThreshold() const {
        return (mCompletionThreshold >= 0) ? mCompletionThreshold : mStartingNumPickups;
    }
    /**
     * @brief Sets #completionThreshold.
     * @param value Integer to set #completionThreshold to
     */
    void setCompletionThreshold(int value);
    /**
    * @brief Returns the registeredActors variable.
    */
    QVariantList registeredActors() {  return mRegisteredActors;  }
    /**
    * @brief Sets the registeredActors variable.
    */
    void setRegisteredActors(QVariantList& registered);
    /**
    *   @brief
    */
    QVariantList levelActorMapping() {  return mLevelActorMapping;  }
    /**
    *   @brief
    */
    void setLevelActorMapping(QVariantList& value);
    /**
     * @brief Returns #portal.
     */
    Actor* getPortal() const { return mPortal; }
    /**
     * @brief Dynamically inserts the specified actor into the level-based scene graph, assuming it
     * does not already exist.
     * @param actor Actor to add to the level's scene graph
     */
    void addActor(Actor* actor);
    /**
     * @brief Returns the set of actors in the level scene graph of the specified type.
     * @param type Type (Actor::ActorType) of Actor%s to get
     */
    const QSet<Actor*>& getActors(Actor::ActorType type) const;
    /**
     * @brief Returns the physics world (@c b2World) owned by this Level.
     */
    b2World* getWorld() const { return mWorld.get(); }
    /**
    * @brief Levels can be managed and others.
    * Note: Managed means, is structured with items defined in qml, with actors.
    * RUBE_BOX2D_LEVEL: Will have or might have a plain box2dworld so you can do anyting with it.
    */
    enum LevelType{
        MANAGED_LEVEL,
        RUBE_BOX2D_LEVEL
    };
    /**
    * @brief Returns the #levelType.
    */
    LevelType levelType() const { return mLevelType; }
    /**
    * @brief Set the #levelType to the @param type;
    */
    void setLevelType(LevelType &type);
    /**
    * @brief Sets the variable mouseToInputArea.
    */
    void setMouseToInputArea(bool value);
    /**
     * @brief Returnt the value of the variable mouseToInputArea.
     */
    bool mouseToInputArea() const { return mMouseToInputArea;}

signals:
    /**
     * @brief Emitted when #cameraBoundary changes.
     */
    void cameraBoundaryChanged();
    /**
     * @brief Emitted when #fillCamera changes.
     */
    void fillCameraChanged();
    /**
     * @brief Emitted when #name changes.
     */
    void nameChanged();
    /**
     * @brief Emitted when #fileName changes.
     */
    void fileNameChanged();
    /**
     * @brief Emitted when #gravity changes.
     */
    void gravityChanged();
    /**
     * @brief Emitted when #parallaxOrigin changes.
     */
    void parallaxOriginChanged();
    /**
     * @brief Emitted when #parallaxStrength changes.
     */
    void parallaxStrengthChanged();
    /**
     * @brief Emitted when #particleLayer changes.
     */
    void particleLayerChanged();
    /**
     * @brief Emitted when #particleAlpha changes.
     */
    void particleAlphaChanged();
    /**
     * @brief Emitted when #particleLayers changes.
     */
    void particleLayersChanged();
    /**
     * @brief Emitted when #completionThreshold changes.
     */
    void completionThresholdChanged();
    /**
    * @brief Emitted when #levelType Changes.
    */
    void levelTypeChanged(LevelType);
    /**
    * @brief Emitted when #jsonData Changes.
    */
    void jsonDataChanged();
    /**
    * @brief Emitted when #jsonFilePath Changes.
    */
    void jsonFilePathChanged();
    /**
    * @brief Emitted when #changeByInput Changes.
    */
    void mouseToInputAreaChanged();
    /**
    * @brief Emitted when registered actors changes.
    */
    void registeredActorsChanged();

protected:
    /**
    * @brief Post-initialization of the Level after all child Actor%s have been created and set.
    */
    virtual void componentComplete() override;
    /**
    * @brief Use to handle managed level.
    */
    inline void managedLevel();
    /**
    * @brief Use to handle loading box2djson format.
    */
    inline void rubeB2DLevel(b2dJson &mBox2dJson);

    //Level Parsing functions.
    /**
    *   @brief
    *   @note This should be only used in the level parsing method,
    *        because it does not check for required variables.
    *       It assumes they are there. Beware!
    */
    inline void actorComponentWithBody(LevelNode* node, QQmlEngine* qmlEngine);
    /**
    *   @brief This is used for creating Actors directly from the rube json object.
    *       Using the actorType property.
    */
    inline void actorFromMapping(b2Body* body, std::string& actorType, QQmlEngine* qmlEngine);
    /**
    *   @brief  This is used when an Actor references directly to b2body by name.
    */
    inline void actorReferenceBody(LevelNode* node);
    /**
    *   @brief This is used by methods used for creating and Actor from qml file and assigning it a Body.
    *   @note initLogic param is used by default, in case the created actors have some logic that
    *   initialized resources after creation. This is purely needed most times.
    */
    inline Actor* createActorComponentWithBody(b2Body* tbody,QString& actorQmlComponent,QQmlEngine* qmlEngine,bool initLogics = true);
    /**
    *   @brief This checks if the b2Body has been initialized as a Body.
    *   if it has it returns that if not it init and returns it.
    */
    Body* checkBodyOrInit(b2Body* b);

private:
    void updateParticleRendererItemZ();

#ifdef BOX2D_LOADER
    b2dJson mBox2dJson;
#endif
    LevelType mLevelType = MANAGED_LEVEL;
    QString mName;
    QString mFileName;
    QString mJsonData;
    QString mJsonFilePath;
    QVariantList mRegisteredActors;
    QVariantList mLevelActorMapping;
    QMap<QString,QString> mLevelActorMap;

    std::unique_ptr<b2World> mWorld;
    QMap<Actor::ActorType, QSet<Actor*>> mActors;
    int mStartingNumPickups = 0;
    int mCompletionThreshold = -1;
    QPointF mGravity = QPointF(0.0f, 9.8f);
    QPointF mParallaxOrigin = QPointF(0.0f, 0.0f);
    QPointF mParallaxStrength = QPointF(1.0f, 0.0f);
    int mParticleLayer = 0;
    float mParticleAlpha = 1.0f;
    float mParticleRadius = 0.05f;
    float mParticleDensity = 1.2f;
    float mParticleGravityScale = 0.4f;
    QList<ParticleLayer*> mParticleLayers;
    ParticleRendererItem* mParticleRendererItem = nullptr;
    QRectF mCameraBoundary = QRectF(
            QPointF(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()),
            QPointF(std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()));
    bool mFillCamera = false;
    bool mMouseToInputArea = false;
    Actor* mPortal = nullptr;

    static const float PARTICLE_RENDERER_LAYER_ZBIAS;
};
Q_DECLARE_METATYPE(Level*)

#endif // LEVEL_H
