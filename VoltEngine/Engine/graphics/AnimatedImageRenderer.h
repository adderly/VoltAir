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

#ifndef ANIMATEDIMAGERENDERER_H
#define ANIMATEDIMAGERENDERER_H

#include "ImageRenderer.h"
#include "AnimatedImageClip.h"
/**
 * @ingroup Engine
 * @ingroup QQuickItem
 * @brief Graphic based on ImageRenderer which displays a frame animation from a texture sheet.
 *
 * The texture sheets used by this class contain a vertical strip of equally sized frames, with
 * the top frame being the first frame. These frames are displayed in sequence, with a specified
 * delay (#frameDelay) between frames.
 *
 * This class is used for various animations in the game, including the Orb collectables, and
 * the Portal animations.
 *
 * @note: If the FrameAmount is changes after the component is initialized, it might have
 *  weird behavior.
 *
 * TODO: FINISH the work with the strips from a subtexture provided with the Texture Packer loader.
 * TODO: fix the calculations done with the width of the texture to count the frames.
 * Ideally the width and/or height should be specified to determine the frames in the texture.
 * TODO: add texture packer loader. And animation from them.
 * If no AnimatedImageClip is defined inside the AnimatedImageRenderer, we create a default one
 * and set it as current.
 * And add it to the AnimatedImageClip list.
 *
 */
// TODO: Supporting grid layouts for texture sheets would allow larger animations to fit into the
// texture sheets within the size limitations of more hardware.

class TexturePackerLoader;

class AnimatedImageRenderer : public ImageRenderer
{
    Q_OBJECT

    /**
    * @brief Propety holding the jsonfilename if specified.
    */
    Q_PROPERTY(QString jsonFilename READ getJsonFilename WRITE setJsonFilename NOTIFY jsonFilenameChanged)
    /**
     * @brief Pause this animation at the current frame.
     */
    Q_PROPERTY(bool paused READ isPaused WRITE setPaused NOTIFY pausedChanged)
    /**
     * @brief Loop this animation, restarting it at the first frame after the last frame has
     * finished displaying.
     */
    Q_PROPERTY(bool looped READ isLooped WRITE setLooped NOTIFY loopedChanged)
    /**
     * @brief Animate this image at the speed of the game, pausing when the game is paused.
     */
    Q_PROPERTY(bool useGameTime READ isUseGameTime WRITE setUseGameTime NOTIFY useGameTimeChanged)
    /**
     * @brief TODO.DOC
     */
    Q_PROPERTY(StripOrientation stripOrientation READ getStripOrientation WRITE setStripOrientation NOTIFY stripOrientationChanged)
    /**
     * @brief Delay between frames, in seconds.
     */
    Q_PROPERTY(float frameDelay READ getFrameDelay WRITE setFrameDelay NOTIFY frameDelayChanged)
    /**
     * @brief Total number of frames in the texture sheet.
     */
    Q_PROPERTY(int frameCount READ getFrameCount WRITE setFrameCount NOTIFY frameCountChanged)
    /**
     * @brief Index of the current frame, zero being the first frame.
     */
    Q_PROPERTY(int currentFrame READ getCurrentFrame WRITE setCurrentFrame)
    /**
     * @brief Seek position into the animation, with @c 0.0f being the start, and @c 1.0f being the
     * end.
     */
    Q_PROPERTY(float currentPosition READ getCurrentPosition WRITE setCurrentPosition)
    /**
     * @brief Don't animate when this image is off screen or #visible is @c false.
     *
     * This reduces the amount of CPU used, but means it isn't completely in step with game time.
     */
    Q_PROPERTY(bool pauseWhenHidden READ getPauseWhenHidden WRITE setPauseWhenHidden
            NOTIFY pauseWhenHiddenChanged)
    /**
     * @brief
     */
    Q_PROPERTY(QVariantList clips READ getClipsAsVariantList
               WRITE setClipsFromVariantList NOTIFY clipsChanged)
    /**
     * @brief Seek position into the animation, with @c 0.0f being the start, and @c 1.0f being the
     * end.
     */
    Q_PROPERTY(QString defaultClip READ getDefaultClip WRITE setDefaultClip)

    /**
    * @brief Defines whether the strip is horizontal or not.
    */
    Q_ENUMS(StripOrientation)

public:
    enum StripOrientation:int{
        HORIZONTAL = 0,
        VERTICAL
    };

    /**
     * @brief Construct a AnimatedImageRenderer.
     * @param parent Parent item
     */
    explicit AnimatedImageRenderer(QQuickItem* parent = nullptr);

    /**
    *   @brief Returns the #jsonFilename.
    */
    QString getJsonFilename() const { return mJsonfilename; }

    /**
    *   @brief Sets the #jsonFilename.
    */
    void setJsonFilename(QString filename);

    /**
     * @brief Returns #paused.
     */
    bool isPaused() const { return mPaused; }
    /**
     * @brief Sets #paused.
     * @param value Boolean to set #paused to
     */
    void setPaused(bool value);
    /**
     * @brief Returns #looped.
     */
    bool isLooped() const { return mLooped; }
    /**
     * @brief Sets #looped.
     * @param value Boolean to set #looped to
     */
    void setLooped(bool value);

    /**
    * @brief Sets the #stripOrientation.
    */
    void setStripOrientation(StripOrientation value);
    /**
    * @brief Returns the strip orientation.
    */
    StripOrientation getStripOrientation() const { return mStripOrientation ;}
    /**
     * @brief Returns #useGameTime.
     */
    bool isUseGameTime() const { return mUseGameTime; }
    /**
     * @brief Sets #useGameTime.
     * @param value Boolean to set #useGameTime to
     */
    void setUseGameTime(bool value);
    /**
     * @brief Returns #frameDelay.
     */
    float getFrameDelay() const { return mFrameDelay; }
    /**
     * @brief Sets #frameDelay.
     * @param value Float to set #frameDelay to
     */
    void setFrameDelay(float value);
    /**
     * @brief Returns #frameCount.
     */
    int getFrameCount() const { return mFrameCount; }
    /**
     * @brief Sets #frameCount.
     * @param value Integer to set #frameCount to
     */
    void setFrameCount(int value);
    /**
     * @brief Returns #currentFrame.
     */
    int getCurrentFrame() const { return mCurrentFrame; }
    /**
     * @brief Sets #currentFrame.
     * @param value Integer to set #currentFrame to
     */
    void setCurrentFrame(int value);
    /**
     * @brief Returns #currentPosition.
     */
    float getCurrentPosition() const { return mCurrentPosition; }
    /**
     * @brief Sets #currentPosition.
     * @param value Float to set #currentPosition to
     */
    void setCurrentPosition(float value);
    /**
     * @brief Returns #pauseWhenHidden.
     */
    bool getPauseWhenHidden() const { return mPauseWhenHidden; }
    /**
     * @brief Sets #pauseWhenHidden.
     * @param value Float to set #pauseWhenHidden to
     */
    void setPauseWhenHidden(bool value);

    /**
    * @brief Sets the clips for the renderer.
    */
    void setClips(const QList<AnimatedImageClip*>& c);

    /**
    * @brief Sets the clips for the renderer.
    */
    void setClips(const QMap<QString,AnimatedImageClip*>& c);

    /**
    * @brief Returns the mClips.
    */
    const QMap<QString,AnimatedImageClip*>& getClips() const;

    /**
    * @brief
    * TODO.FIXME: find another way for the unneeded tmp.
    */
    void setClipsFromVariantList(const QVariantList& c);

    /**
    * @brief Return the clips as variant list.
    * @note used with the goodies of qml.
    */
    QVariantList getClipsAsVariantList() const;

    /**
    *   @brief Find a clip by name, then set it as current.
    */
    Q_INVOKABLE void setCurrentClipByName(QString clipName);

    /**
    *   @brief Set the DefaultClip.
    */
    void setDefaultClip(QString c);

    /**
    *   @brief Returns the defaultClip;
    */
    QString getDefaultClip() const { return mDefaultClip;  }

signals:
    /**
     * @brief Emitted when #paused changes.
     */
    void pausedChanged();
    /**
     * @brief Emitted when #looped changes.
     */
    void loopedChanged();
    /**
     * @brief Emitted when #verticalStrip changes.
     */
    void stripOrientationChanged();
    /**
     * @brief Emitted when #useGameTime changes.
     */
    void useGameTimeChanged();
    /**
     * @brief Emitted when #frameDelay changes.
     */
    void frameDelayChanged();
    /**
     * @brief Emitted when #frameCount changes.
     */
    void frameCountChanged();
    /**
     * @brief Emitted when the last frame ends, or the animation has just looped.
     */
    void animationReachedEnd();
    /**
     * @brief Emitted when #pauseWhenHidden changes.
     */
    void pauseWhenHiddenChanged();
    /**
    *   @brief Emitted when the clips changed.
    */
    void clipsChanged();
    /**
    * @brief Emitted when current clip Changed.
    */
    void currentClipChanged();
    /**
    *   @brief Emitted when default clip changed.
    */
    void defaultClipChanged();
    /**
    *   @brief Emitted when the #Jsonfile changes.
    */
    void jsonFilenameChanged();

protected:
    virtual void synchronizeForRendering(RenderList* renderList) override;
    virtual void computeDestTextureSize(QSizeF* textureSize) const override;
    virtual void computeSourceTextureRect(QRectF* textureSubRect) const override;
    virtual void componentComplete() override;

    /**
    * @brief Find the children of type AnimatedImageClip.
    */
    void findChildClips();
    /**
    *   @brief Set the currentClip.
    */
    void setCurrentClip(AnimatedImageClip* c,bool restart = false);

private:
    void moveCurrentFrame(int frame);
    void updateAnimation();

    //Texture Packer loader
    QString mJsonfilename;
    AnimatedImageClip::ClipType clipType = AnimatedImageClip::STRIP;

    //clip support
    QString mDefaultClip = "default";
    float mCurrentClipDelay = 0.0f;
    bool mFrameRotation = false;//when the image is rotated in the texturepacker
    AnimatedImageClip* mCurrentClip = nullptr;
    QMap<QString,AnimatedImageClip*> mClips;

    StripOrientation mStripOrientation = StripOrientation::VERTICAL;
    bool mPaused = false;
    bool mLooped = true;
    bool mUseGameTime = true;
    bool mPauseWhenHidden = false;
    bool mAtEnd = false;
    float mFrameDelay = 0.0f;
    int mFrameCount = 0;
    float mNormalizedInvFrameCount = 1.0f;
    float mFrameTimer = 0.0f;
    int mCurrentFrame = 0;
    float mCurrentPosition = 0.0f;
};

#endif // ANIMATEDIMAGERENDERER_H
