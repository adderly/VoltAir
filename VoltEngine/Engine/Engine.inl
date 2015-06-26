#include "Engine.h"
#include "Util.h"

inline void Engine::loadEngineSettings()
{
    if(!mUseSettings)
        return;

    mSettings.reset(new QSettings(Util::getUrlPathToAsset("assets/config/engine.config").toLocalFile(),
                          QSettings::IniFormat,this));

    qDebug() << "settings status = " << mSettings->status();

    if(mSettings->status() == QSettings::NoError){
        defaultW = mSettings->value("width",defaultW).toInt();
        defaultH = mSettings->value("height",defaultH).toInt();
        mSoundsMuted = (mSettings->value("sound",1).toInt() != 1);
        mMusicVol = mSettings->value("music_vol",1.0).toFloat() ;
        mSfxVol = mSettings->value("sfx_vol",1.0).toFloat();
        mDebug =  (mSettings->value("debug","false").toString() == "true");
        mCloudDataEnabled = (mSettings->value("cloud_data","false").toString() == "true");
    }
}



