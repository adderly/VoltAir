#ifndef ANIMATEDIMAGECLIP_H
#define ANIMATEDIMAGECLIP_H

#include <QQuickItem>
/**
*   @brief Class used in #AnimatedImageRenderer to identify animation clips using names,
*   in a common image strip. Also provides a nice way using the TexturePackerLoader,
*   for rendering clips in different parts of a spritesheet using the data
*   provided by the loader.
*
*   Note: Each clip, should have a amount of frames. #mFrameAmount.
*/
class AnimatedImageClip : public QQuickItem
{
    Q_OBJECT

    /**
    *   @brief Property used for identifing the clip in the renderer.
    */
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    /**
    *   @brief Type of the clip, defining the Type of frames it manages.
    */
    Q_PROPERTY(ClipType clipType READ getClipType WRITE setClipType NOTIFY clipTypeChanged)
    /**
    *   @brief The frames for the clip.
    */
    Q_PROPERTY(QVariantList frames READ getFramesAsVariantList WRITE setFramesAsVariantList NOTIFY framesChanged)
    /**
    *   @brief Defines whether the clip has a custom source different from the AnimatedImageRenderer.
    */
    Q_PROPERTY(QString customSource READ getCustomSource WRITE setCustomSource NOTIFY customSourceChanged)
    /**
    *   @brief Used to control frame rendering and change.
    */
    Q_PROPERTY(int frameCount READ getFrameCount WRITE setFrameCount NOTIFY frameCountChanged)
    /**
    *   @brief Contains the amount (if specified) of frames in a specific image strip.
    */
    Q_PROPERTY(int stripFrameAmount READ getStripFrameAmount WRITE setStripFrameAmount NOTIFY stripFrameAmountChanged)
    /**
    *   @brief Used to control where in the strip we want to render from.
    *   @note When we advance rendering in the strip, we must know where in the strip is the
    *         frame. This is used to fallow down the strip and getting the frame at the "offset" of the strip.
    */
    Q_PROPERTY(int frameOffset READ getFrameOffset WRITE setFrameOffset NOTIFY frameOffsetChanged)
    /**
    *   @brief The delay of the frames for changing.
    */
    Q_PROPERTY(float frameDelay READ getFrameDelay WRITE setFrameDelay NOTIFY frameDelayChanged)
    /**
    *   @brief
    */
    Q_ENUMS(ClipType)

public:
    AnimatedImageClip(QQuickItem* parent = nullptr);
    ~AnimatedImageClip();

    /**
    *   @brief The type of clip this is.
    *   @note This type is used to identify which method to use for rendering.
    *   STRIP means the animationś frames are in one image or portion of an image.
    *   SINGLE_FRAME_STRIP means is a strip but takes the image from a section of an image(spritesheet).
    *   MULTI_FRAME means the animation is made from multiple sections of and image (spritesheet).
    */
    enum ClipType{
        STRIP,SINGLE_FRAME_STRIP,MULTI_FRAME
    };

    /**
    * @brief Sets the #name of the clip.
    */
    void setName(const QString value);

    /**
    * @brief Return the name of the clip.
    */
    QString getName() const { return mName; }

    /**
    * @brief Sets the #clipType.
    */
    void setClipType(ClipType type);

    /**
    * @brief Returns the #clipType.
    */
    ClipType getClipType() { return mCliptype; }

    /**
    * @brief sets the #frames for the clip.
    */
    void setFramesAsVariantList(QVariantList list);

    /**
    * @brief Returns the #frames of the clip.
    */
    QVariantList getFramesAsVariantList();

    /**
    * @brief sets the #frames for the clip.
    */
    void setFrames(QList<QString>& list);

    /**
    * @brief Returns the #frames of the clip.
    */
    QList<QString> getFrames() { return mFrames; }

    /**
    * @brief Sets #customSource.
    */
    void setCustomSource(const QString value);

    /**
    * @brief Return the #customSource.
    */
    QString getCustomSource() const { return mCustomSource; }

    /**
    * @brief Sets the #frameCount.
    */
    void setFrameCount(int value);

    /**
    * @brief Returns the #frameCount.
    */
    int getFrameCount() const { return mFrameCount; }

    /**
    * @brief Sets the #stripOrientation.
    */
    void setStripOrientation(int value);

    /**
    * @brief Return the #stripOrientation.
    */
    int getStripOrientation() const { return mStripOrientation; }

    /**
    * @brief Returns whether the #customSource has contents.
    */
    bool hasCustomSource() const { return !mCustomSource.isEmpty() ;}

    /**
    * @brief Sets the #frameOffset.
    */
    void setFrameOffset(int value);

    /**
    * @brief Returns the #frameOffset.
    */
    int getFrameOffset() const { return mFrameOffset; }

    /**
    * @brief Sets the #stripAmount.
    */
    void setStripFrameAmount(int amount) ;

    /**
    * @brief Return the #stripAmount.
    */
    int getStripFrameAmount() const { return mStripFrameAmount;}

    /**
    * @brief Sets the #maxClipAmount.
    */
    void setClipMaxCount(int amount) { mClipMaxCount = amount;}

    /**
    * @brief Returns the #maxClipAmount.
    */
    int getClipMaxCount() const { return mClipMaxCount;}

    /**
    * @brief Sets the #frameDelay.
    */
    void setFrameDelay(float value);

    /**
    * @brief Returns the #frameDelay.
    */
    float getFrameDelay()const { return mFrameDelay; }

    /**
    *   @brief AnimatedImageRenderer is our homie.
    */
    friend class AnimatedImageRenderer;

signals:
    void nameChanged();
    void stripOrientationChanged();
    void customSourceChanged();
    void frameCountChanged();
    void frameOffsetChanged();
    void stripFrameAmountChanged();
    void frameDelayChanged();
    void clipTypeChanged();
    void framesChanged();

public slots:


private:
    //the clip can have a custom image source;
    QString mCustomSource;
    //Identifies the clip type for rendering
    ClipType mCliptype = STRIP;

    //
    /**
    *  @note support for spritesheet frames.
    *  The Frames, FrameRects, FrameRotations and FrameImage size
    *  provides information for rendering from a file loaded with
    *  the TexturePackerLoader.
    */
    QList<QString> mFrames;
    QList<QRect>   mFrameRects;
    QList<bool>    mFrameRotations;
    QSize          mFrameImageSize;

    //0 = Horizontal, 1 = vertical.
    int mStripOrientation = 0;
    //clip name.
    QString mName;
    //used to identify from AnimatedImageRenderer will be used.
    int mSouceId;
    // the delay, if set to zero the AnimatedImageRenderer
    float mFrameDelay = 0.0f;
    //When using a custom source this needs to be the amount of frames
    //the strip has, which it will be used to find the clip offset, if it is not the whole strip.
    int mStripFrameAmount = 0;
    //holds the max amount of frames in which this can go. frameCount + offset
    int mClipMaxCount = 0;
    //the amount of frames the clip has.
    int mFrameCount = 0;
    //when dealing with shared source, this is used to identify where
    //the clip begins in the shared strip.
    int mFrameOffset = 0;

    //Used when a portion of a texture will be used.
    //Normaly it uses a whole texture for one animation
    //but this implies that a huge texture could have many animations.
    int mTextureOffsetX = 0;
    int mTextureOffsetY = 0;

};
#endif // ANIMATEDIMAGECLIP_H
