#include "AnimatedImageClip.h"

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


