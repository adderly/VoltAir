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
#include "utils/TexturePackerLoader.h"

AnimatedImageRenderer::AnimatedImageRenderer(QQuickItem* parent)
    : ImageRenderer(parent) {
}

void AnimatedImageRenderer::setJsonFilename(QString filename){
    mJsonfilename = filename;
    emit jsonFilenameChanged();
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

void AnimatedImageRenderer::moveCurrentFrame(int frame) {
    mCurrentFrame = frame;
    mCurrentPosition = frame * mNormalizedInvFrameCount;
    markSourceTextureRectDirty();
}

void AnimatedImageRenderer::setPauseWhenHidden(bool value) {
    mPauseWhenHidden = value;
    emit pauseWhenHiddenChanged();
}

void AnimatedImageRenderer::setClips(const QList<AnimatedImageClip*>& c){
    QMap<QString,AnimatedImageClip*> tmp ;
    for(auto clip : c){
        tmp.insert(clip->getName(),clip);
    }
    mClips = tmp;
    emit clipsChanged();
}

void AnimatedImageRenderer::setClips(const QMap<QString, AnimatedImageClip*>& c){
    mClips = c;
    emit  clipsChanged();
}

const QMap<QString, AnimatedImageClip*>& AnimatedImageRenderer::getClips() const{
    return mClips;
}

void AnimatedImageRenderer::setClipsFromVariantList(const QVariantList &c){
    QList<QObject*> dummy = Util::toList<QObject*>(c);
    QMap<QString,AnimatedImageClip*> tmp;

    AnimatedImageClip* clip = nullptr;
    for(QObject* obj:dummy){
        clip = qobject_cast<AnimatedImageClip*>(obj);
        tmp.insert(clip->getName(),clip);
    }
    setClips(tmp);
}

QVariantList AnimatedImageRenderer::getClipsAsVariantList() const{
    auto l = mClips.values();
    return Util::toVariantList(l);
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

    if(clipType == AnimatedImageClip::STRIP)
    {
        if(mStripOrientation == StripOrientation::VERTICAL){
            textureSize->setHeight(textureSize->height() * mNormalizedInvFrameCount);
        }else{
            textureSize->setWidth(textureSize->width() * mNormalizedInvFrameCount);
        }
    }else if(clipType == AnimatedImageClip::SINGLE_FRAME_STRIP){
        if(mCurrentClip){
            QRect r = mCurrentClip->mFrameRects[0];
            if(mStripOrientation == StripOrientation::VERTICAL){
                textureSize->setWidth(r.width());
                textureSize->setHeight(r.height() * mNormalizedInvFrameCount);
            }else{
                textureSize->setWidth(r.width() * mNormalizedInvFrameCount);
                textureSize->setHeight(r.height());
            }
        }
    }else if(clipType == AnimatedImageClip::MULTI_FRAME){
        if(mCurrentClip){
            QRect r = mCurrentClip->mFrameRects[mCurrentFrame];
            auto thiz = const_cast<AnimatedImageRenderer*>(this);
            if(!mCurrentClip->mFrameRotations[mCurrentFrame]){
                textureSize->setWidth(r.width());
                textureSize->setHeight(r.height());
                thiz->setRotation(0);
                thiz->mFrameRotation = false;
            }else{
                textureSize->setWidth(r.height());
                textureSize->setHeight(r.width());
                thiz->setRotation(-90);
                thiz->mFrameRotation = true;
            }
        }
    }
}

void AnimatedImageRenderer::computeSourceTextureRect(QRectF* textureSubRect) const {
    float frameSize;
    float offset;

    if(clipType == AnimatedImageClip::STRIP )
    {
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
    }else if(clipType ==  AnimatedImageClip::SINGLE_FRAME_STRIP){

        QRect r = mCurrentClip->mFrameRects[0];

        if(mStripOrientation == StripOrientation::VERTICAL){
            frameSize = textureSubRect->height() * mNormalizedInvFrameCount;
            offset = frameSize * mCurrentFrame;
            textureSubRect->setHeight(frameSize);
            textureSubRect->moveBottom(textureSubRect->bottom() + offset);
        }else{

            float framex = 0.0f;
            float framey = 0.0f;
            float framew = 0.0f;
            float frameh = 0.0f;
            float normw =  0.0f;
            float normh =  0.0f;

            normw =  (float) 1/mCurrentClip->mFrameImageSize.width();
            normh =  (float) 1/mCurrentClip->mFrameImageSize.height();

            framex = (float) r.x() * normw;
            framey = (float) r.y() * normh;
            framew = (float) r.width() * normw;
            frameh = (float) r.height() * normh;

            frameSize = framew * mNormalizedInvFrameCount;

            offset = frameSize * mCurrentFrame;
            textureSubRect->setX(framex+offset);
            textureSubRect->setY(framey);
            textureSubRect->setWidth(framew);
            textureSubRect->setHeight(frameh);
            //textureSubRect->moveRight(textureSubRect->right() + offset);
        }
    }else if(clipType == AnimatedImageClip::MULTI_FRAME){
        if(mCurrentClip){
            QRect r = mCurrentClip->mFrameRects[mCurrentFrame];

            float framex = 0.0f;
            float framey = 0.0f;
            float framew = 0.0f;
            float frameh = 0.0f;
            float normw =  0.0f;
            float normh =  0.0f;

            normw =  (float) 1/mCurrentClip->mFrameImageSize.width();
            normh =  (float) 1/mCurrentClip->mFrameImageSize.height();
            framex = (float) r.x() * normw;
            framey = (float) r.y() * normh;

            if(!mFrameRotation){
                framew =  (float) (r.width() )* normw ;
                frameh =  (float) (r.height())* normh ;
            }else{
                framew =  (float) (r.height() )* normw ;
                frameh =  (float) (r.width())* normh ;
            }
            textureSubRect->setX(framex);
            textureSubRect->setY(framey);
            textureSubRect->setWidth(framew);
            textureSubRect->setHeight(frameh);
        }
    }
}

void AnimatedImageRenderer::componentComplete(){
    ImageRenderer::componentComplete();
    findChildClips();
}

void AnimatedImageRenderer::findChildClips()
{
    setVisible(false);
    mPaused = true;

    TP_Document* tp_doc = nullptr;
    //load json file.
    if(!mJsonfilename.isEmpty()) {
        tp_doc = TexturePackerLoader::getInstance()->getTPDocument(mJsonfilename);
        if(tp_doc)  setSource(tp_doc->image_path);
    }

    QList<AnimatedImageClip*> clips = findChildren<AnimatedImageClip*>();

    for(AnimatedImageClip* clip:clips)
    {
        if(!clip->hasCustomSource()){
            clip->setStripFrameAmount(mFrameCount);
            clip->setCustomSource(getSource());
        }

        if(tp_doc && (clip->getClipType() == AnimatedImageClip::MULTI_FRAME ||
                      clip->getClipType() == AnimatedImageClip::SINGLE_FRAME_STRIP) )
        {
            TP_Image* tp_img = nullptr;
            QRect frameRectSize;
            bool rotated;
            for(QString frameName:clip->mFrames)
            {
                rotated = false;
                tp_img = tp_doc->m_images[frameName];
                if(tp_img)
                {
                    frameRectSize = tp_img->frame;
                    rotated = tp_img->rotated;
                    clip->mFrameImageSize.setWidth(tp_doc->size[0]);
                    clip->mFrameImageSize.setHeight(tp_doc->size[1]);
                }
                clip->mFrameRects.append(frameRectSize);
                clip->mFrameRotations.append(rotated);
            }
            if(clip->getClipType() == AnimatedImageClip::MULTI_FRAME){
                clip->mFrameCount = clip->mFrames.size();
            }else{
                clip->setStripFrameAmount(clip->mFrameCount);
            }
            qDebug() << "Frames : " << clip->mFrames;
            qDebug() << "ImageSizes: " << clip->mFrameImageSize;
            qDebug() << "FrameRects: " << clip->mFrameRects;
            qDebug() << "FrameRotation: " << clip->mFrameRotations;
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
    setVisible(true);
}

void AnimatedImageRenderer::setCurrentClipByName(QString clipName)
{
    mPaused = true;

    qDebug() << "setCurrentClip = " << clipName;
    AnimatedImageClip* newCurrent = mClips[clipName];

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

    clipType = mCurrentClip->mCliptype;

    mCurrentClip->setClipMaxCount( mCurrentClip->getFrameCount()
                                   + mCurrentClip->getFrameOffset());

    //TODO: if clip has custom source, the AnimatedRenerer must use
    // this source frame count.
    if(clipType == AnimatedImageClip::MULTI_FRAME){
        setFrameCount(mCurrentClip->getFrameCount());
    }else if(clipType == AnimatedImageClip::SINGLE_FRAME_STRIP ||
             mCurrentClip->hasCustomSource()){
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
