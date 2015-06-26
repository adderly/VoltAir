#include "AnimatedImageClip.h"
#include "utils/Util.h"

AnimatedImageClip::AnimatedImageClip(QQuickItem *parent)
    :QQuickItem(parent)
{

}

AnimatedImageClip::~AnimatedImageClip(){
}

void AnimatedImageClip::setName(const QString value){
    mName = value;
    emit nameChanged();
}

void AnimatedImageClip::setClipType(AnimatedImageClip::ClipType type){
    mCliptype = type;
    emit clipTypeChanged();
}

void AnimatedImageClip::setFramesAsVariantList(QVariantList list){
    QList<QString> tmp = Util::toList<QString>(list);
    setFrames(tmp);
}

QVariantList AnimatedImageClip::getFramesAsVariantList(){
    return Util::toVariantList(mFrames);
}

void AnimatedImageClip::setFrames(QList<QString>& list){
    mFrames = list;
    emit framesChanged();
}

void AnimatedImageClip::setCustomSource(const QString value){
    mCustomSource = value;
    emit customSourceChanged();
}

void AnimatedImageClip::setFrameCount(int value){
    mFrameCount = value;
    emit frameCountChanged();
}

void AnimatedImageClip::setStripOrientation(int value){
    mStripOrientation = value;
    emit stripOrientationChanged();
}

void AnimatedImageClip::setFrameOffset(int value){
    mFrameOffset = value;
    emit frameOffsetChanged();
}

void AnimatedImageClip::setStripFrameAmount(int amount){
    mStripFrameAmount = amount;
    emit stripFrameAmountChanged();
}

void AnimatedImageClip::setFrameDelay(float value){
    mFrameDelay = value;
    emit frameDelayChanged();
}


