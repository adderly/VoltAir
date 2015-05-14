#include "Engine.h"
#include "Util.h"

inline void Engine::loadEngineSettings()
{
    if(!mUseSettings)
        return;

    mSettings.reset(new QSettings(Util::getUrlPathToAsset("assets/config/engine.config").toLocalFile(),
                          QSettings::IniFormat,this));

    qDebug() << "settings status = " << mSettings->status();

    int w = 0;
    int h = 0;
    bool muted = false;
    qreal music_vol = 1.0;
    qreal sfx_vol = 1.0;
    bool debug = false;
    //TODO:add other properties to Engine.
//    mSettings->

    if(mSettings->status() == QSettings::NoError){
        w = mSettings->value("width",defaultW).toInt();
        h = mSettings->value("height",defaultH).toInt();
        muted = (mSettings->value("sound",1).toInt() != 1);
        music_vol = mSettings->value("music_vol",1.0).toFloat() ;
        sfx_vol = mSettings->value("sfx_vol",1.0).toFloat() ;
        debug = (mSettings->value("debug","false").toString() == "true");

        defaultW = w;
        defaultH = h;
        mSoundsMuted = muted;
        mMusicVol = music_vol;
        mSfxVol = sfx_vol;
        mDebug = debug;
    }

}



