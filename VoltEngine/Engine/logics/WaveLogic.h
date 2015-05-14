#ifndef WAVELOGIC_H
#define WAVELOGIC_H

#include <Logic.h>

class Wave;
class Actor;


/**
*   @brief Logic for handling wave like games. This interacts with the level and the specified
*          behavior. For loading required resources or simple levels.
*   @notes Maybe actors need some other control for this.
*/
class WaveLogic: public Logic
{
    Q_OBJECT
public:
    WaveLogic(QObject* parent = nullptr);
    ~WaveLogic();

    /**
    *   @brief Initialize the current wave, acquiring and indexing resources.
    */
    void initWave();

    /**
    *   @brief In case of pause or user input interruption this pause the wave till
    *          the contrary is specified.
    */
    void pauseWave();

    /**
    *   @brief If for some reason the wave was pause this assure it not anymore.
    */
    void continueWave();

    /**
    *   @brief If the wave is over or some other reason for ending this would be implemented here.
    */
    void endWave();

    /**
    *   @brief Add the logic initialization impl.
    */
    void init() override;

    /**
    *   @brief Main logic control center, things are really happening here.
    */
    void update() override;

    /**
    *   @brief Waves make actors react, appear or say something.
    */
    void activateActor(Actor* actor);

    /**
    *   @brief Maybe a group of actor would like to appear or so.
    *   @note  Ex: Enemies coming from the north! Get ready!.
    */
    void activateActors(QList<Actor*> actors);

private:
    Wave* mWave = nullptr;
    std::vector<Wave*> mLevelWaves;
};

/**
*   @brief Holds the metadata of the wave, which is later used to bring everything to life.
*/
class Wave: public QObject{
    Q_OBJECT
public :
    Wave(QObject* parent = nullptr):
        QObject(parent){

    }

private:

};

#endif // WAVELOGIC_H
