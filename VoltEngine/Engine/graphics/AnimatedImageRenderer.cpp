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

#include "AnimatedImageRenderer.h"
#include "Engine.h"
#include "utils/Util.h"
#include "AnimatedImageClip.h"

AnimatedImageRenderer::AnimatedImageRenderer(QQuickItem* parent)
    : ImageRenderer(parent) {
}

void AnimatedImageRenderer::setPaused(bool value) {
    mPaused = value;
    emit pausedChanged();
}

void AnimatedImageRenderer::setLooped(bool value) {
    mLooped = value;
    emit loopedChanged();
}

void AnimatedImageRenderer::setStripOrientation(AnimatedImageRenderer::StripOrientation value){
    mStripOrientation = value;
    emit stripOrientationChanged();
}

void AnimatedImageRenderer::setUseGameTime(bool value) {
    mUseGameTime = value;
    emit useGameTimeChanged();
}

void AnimatedImageRenderer::setFrameDelay(float value) {
    mFrameDelay = value;
    emit frameDelayChanged();
}

void AnimatedImageRenderer::setDefaultClip(QString c){
    mDefaultClip = c;
    emit defaultClipChanged();
}

void AnimatedImageRenderer::setFrameCount(int value) {
    mFrameCount = value;
    if (mFrameCount > 0) {
        // Note: Not numerically stable, but better than doing divides every frame change and works
        // all right for small frame counts.
        mNormalizedInvFrameCount = 1.0f / mFrameCount;
    } else {
        mNormalizedInvFrameCount = 1.0f;
    }
    emit frameCountChanged();
}

void AnimatedImageRenderer::setCurrentFrame(int value) {
    int nextFrame = value;
    if (mFrameCount > 0) {
        nextFrame = value % mFrameCount;
    }
    moveCurrentFrame(nextFrame);
}

void AnimatedImageRenderer::setCurrentPosition(float value) {
    int nextFrame = 0;
    if (mFrameCount > 0) {
        nextFrame = qMax(0, qMin(mFrameCount - 1, int(floorf(value * mFrameCount))));
    }
    moveCurrentFrame(nextFrame);
}

void AnimatedImageRenderer::setPauseWhenHidden(bool value) {
    mPauseWhenHidden = value;
    emit pauseWhenHiddenChanged();
}

void AnimatedImageRenderer::setClips(const QList<AnimatedImageClip *> &c){
    mClips = c;
    emit  clipsChanged();
}

const QList<AnimatedImageClip *> &AnimatedImageRenderer::getClips() const{
    return mClips;
}

void AnimatedImageRenderer::setClipsFromVariantList(const QVariantList &c){
    QList<QObject*> dummy = Util::toList<QObject*>(c);
    QList<AnimatedImageClip*> tmp;

    for(QObject* obj:dummy){
        tmp.append(qobject_cast<AnimatedImageClip*>(obj));
    }
    setClips(tmp);
}

QVariantList AnimatedImageRenderer::getClipsAsVariantList() const{
    return Util::toVariantList(mClips);
}

void AnimatedImageRenderer::synchronizeForRendering(RenderList* renderList) {
    // Update our animation.
    // TODO: Note, this is bad manners to do in a renderer sync.
    if (!(mPauseWhenHidden && (isCulled() || !isVisible()))) {
        updateAnimation();
    }

    return ImageRenderer::synchronizeForRendering(renderList);
}

void AnimatedImageRenderer::computeDestTextureSize(QSizeF* textureSize) const {

    if(mStripOrientation == StripOrientation::VERTICAL){
        textureSize->setHeight(textureSize->height() * mNormalizedInvFrameCount);
    }else{
        textureSize->setWidth(textureSize->width() * mNormalizedInvFrameCount);
    }
}

void AnimatedImageRenderer::computeSourceTextureRect(QRectF* textureSubRect) const {
    float frameSize;
    float offset;
    if(mStripOrientation == StripOrientation::VERTICAL){
        frameSize = textureSubRect->height() * mNormalizedInvFrameCount;
        offset = frameSize * mCurrentFrame;
        textureSubRect->setHeight(frameSize);
        textureSubRect->moveBottom(textureSubRect->bottom() + offset);
    }else{
        frameSize = textureSubRect->width() * mNormalizedInvFrameCount;
        offset = frameSize * mCurrentFrame;
        textureSubRect->setWidth(frameSize);
        textureSubRect->moveRight(textureSubRect->right() + offset);
    }
}

void AnimatedImageRenderer::componentComplete(){
    ImageRenderer::componentComplete();
    findChildClips();
}

void AnimatedImageRenderer::findChildClips(){
    mPaused = true;
    QList<AnimatedImageClip*> clips = findChildren<AnimatedImageClip*>();

    for(AnimatedImageClip* clip:clips){
        if(!clip->hasCustomSource()){
            clip->setStripFrameAmount(mFrameCount);
            clip->setCustomSource(getSource());
        }
        qDebug() << "Anim : "<< clip->getName();
    }
    //add the default clip.
    if(clips.isEmpty()){
        AnimatedImageClip* def = new AnimatedImageClip();
        def->setName(mDefaultClip);
        def->setFrameCount(mFrameCount);
        def->setFrameOffset(0);
        clips.append(def);
    }//check if there is no specified clip.
    else if(mDefaultClip.isEmpty() || mDefaultClip == "default"){
        qWarning() << " defaultClip is empty, please specified one clip.";
    }

    setClips(clips);
    setCurrentClipByName(mDefaultClip);
    mPaused = false;
}

void AnimatedImageRenderer::setCurrentClipByName(QString clipName)
{
    mPaused = true;

    qDebug() << "setCurrentClip = " << clipName;
    AnimatedImageClip* newCurrent = nullptr;
    for(int n = 0;n < mClips.size();n++){
        if(mClips[n]->getName() == clipName){
            newCurrent = mClips[n];
            break; //no need to continue
        }
    }

    if(!newCurrent){
         setVisible(false);
         mPaused = false;
        qDebug() << "Clip not found = " << clipName;
        return;
    }else{
         setVisible(true);
    }
    setCurrentClip(newCurrent);
    mPaused = false;
}

void AnimatedImageRenderer::setCurrentClip(AnimatedImageClip *c, bool restart){

    if(mCurrentClip == c){
        if(restart) setCurrentFrame(mCurrentClip->getFrameOffset());
        return;
    }
    mCurrentClip = c;
    mCurrentClip->setClipMaxCount( mCurrentClip->getFrameCount()
                                   + mCurrentClip->getFrameOffset());

    //TODO: if clip has custom source, the AnimatedRenerer must use
    // this source frame count.
    if(mCurrentClip->hasCustomSource()){
        setFrameCount(mCurrentClip->getStripFrameAmount());
    }
    setSource(mCurrentClip->getCustomSource());

    //use the clip delay, cause its specified
    if(mCurrentClip->getFrameDelay() > 0.0f){
        mCurrentClipDelay = mCurrentClip->getFrameDelay();
    }else{
       mCurrentClipDelay = mFrameDelay;
    }
    setCurrentFrame(mCurrentClip->getFrameOffset());
    emit currentClipChanged();
}

void AnimatedImageRenderer::moveCurrentFrame(int frame) {
    mCurrentFrame = frame;
    mCurrentPosition = frame * mNormalizedInvFrameCount;
    markSourceTextureRectDirty();
}

void AnimatedImageRenderer::updateAnimation() {
    bool isPausedFromGame = mUseGameTime && Engine::getInstance()->isPaused();
    if (isPaused() || mFrameCount <= 0 || isPausedFromGame
            || !mCurrentClip ) {
        return;
    }

    mFrameTimer += Engine::TIME_STEP_S;
    if (mFrameTimer > mCurrentClipDelay) {
        int clipMaxCount = mCurrentClip->getClipMaxCount();
        int oldFrame = mCurrentFrame;
        int nextFrame = oldFrame;
        int frameSteps = int(mFrameTimer / mCurrentClipDelay);
        mFrameTimer -= frameSteps * mCurrentClipDelay;
        nextFrame += frameSteps;

        bool reachedEnd = false;

        if(nextFrame >= clipMaxCount){
            reachedEnd = true;
            if (isLooped()) {
                nextFrame += mCurrentClip->getFrameOffset();
                nextFrame %= clipMaxCount;
            } else {
                nextFrame = clipMaxCount - 1;
            }
        }

        if (nextFrame != oldFrame) {
            moveCurrentFrame(nextFrame);
        }
        if (reachedEnd != mAtEnd) {
            mAtEnd = reachedEnd;
            if (mAtEnd) {
                emit animationReachedEnd();
            }
        }
    }
}
