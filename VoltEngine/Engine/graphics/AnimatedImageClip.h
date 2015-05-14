#ifndef ANIMATEDIMAGECLIP_H
#define ANIMATEDIMAGECLIP_H

#include <QQuickItem>
/**
*   @brief Class used in #AnimatedImageRenderer to identify animation clips,
*   in a common image strip.
*
*   Note: Each clip, should have a amount of frames. #mFrameAmount
*/
class AnimatedImageClip : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(QString customSource READ getCustomSource WRITE setCustomSource NOTIFY customSourceChanged)

    Q_PROPERTY(int frameCount READ getFrameCount WRITE setFrameCount NOTIFY frameCountChanged)

    Q_PROPERTY(int stripFrameAmount READ getStripFrameAmount WRITE setStripFrameAmount NOTIFY stripFrameAmountChanged)

    Q_PROPERTY(int frameOffset READ getFrameOffset WRITE setFrameOffset NOTIFY frameOffsetChanged)

    Q_PROPERTY(float frameDelay READ getFrameDelay WRITE setFrameDelay NOTIFY frameDelayChanged)

public:
    AnimatedImageClip(QQuickItem* parent = nullptr);
    ~AnimatedImageClip();

    /**
    * @brief
    */
    void setName(const QString value);

    /**
    * @brief
    */
    QString getName() const { return mName; }

    /**
    * @brief
    */
    void setCustomSource(const QString value);

    /**
    * @brief
    */
    QString getCustomSource() const { return mCustomSource; }

    /**
    * @brief
    */
    void setFrameCount(int value);

    /**
    * @brief
    */
    int getFrameCount() const { return mFrameCount; }
    /**
    * @brief
    */
    void setStripOrientation(int value);

    /**
    * @brief
    */
    int getStripOrientation() const { return mStripOrientation; }

    /**
    * @brief
    */
    bool hasCustomSource() const { return !mCustomSource.isEmpty() ;}

    /**
    * @brief
    */
    void setFrameOffset(int value);

    /**
    * @brief
    */
    int getFrameOffset() const { return mFrameOffset; }

    /**
    * @brief
    */
    void setStripFrameAmount(int amount) ;

    /**
    * @brief
    */
    int getStripFrameAmount() const { return mStripFrameAmount;}

    /**
    * @brief
    */
    void setClipMaxCount(int amount){ mClipMaxCount = amount;}

    /**
    * @brief
    */
    int getClipMaxCount() const { return mClipMaxCount;}

    /**
    * @brief
    */
    void setFrameDelay(float value);

    /**
    * @brief
    */
    float getFrameDelay()const { return mFrameDelay; }

signals:
    void nameChanged();
    void stripOrientationChanged();
    void customSourceChanged();
    void frameCountChanged();
    void frameOffsetChanged();
    void stripFrameAmountChanged();
    void frameDelayChanged();

public slots:


private:
    //the clip can have a custom image source;
    QString mCustomSource;
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
