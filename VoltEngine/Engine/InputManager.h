#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GameInput/ControllerManager.h>
#include <QList>
#include <QObject>
#include <QMap>

class VirtualController;

/**
 * @brief Manages a mapping from unique player identifiers to different controllers that connect and
 * disconnect throughout the game.
 *
 * Additionally, this class abstracts the different controller capabilities away from the game,
 * instead defining and allowing the querying of different player action values.
 * @note PlayerManager currently supports the following controller "types": gamepads, touch
 * navigation devices, and keyboards.
 */
class InputManager: public QObject, private ControllerManager::DeviceConnectionListener {
    Q_OBJECT
public:
    InputManager(QObject* parent =  nullptr);
    virtual ~InputManager();

    /**
     * @brief Init function when subclassed  could be used to initialize input properties.
     */
    virtual void init();

    /**
     * @brief Returns whether or not @p player is available (i.e. currently connected with a
     * controller).
     */
    bool isPlayerAvailable(int player) const { return mPlayerToDeviceMap.contains(player); }
    /**
     * @brief Returns the list of currently available players (i.e. players currently connected with
     * a controller).
     */
    QList<int> getAvailablePlayers() const { return mPlayerToDeviceMap.keys(); }
    /**
     * @brief Resets all the player <-> controller mappings.
     * @note This method will also reset all the associated VirtualController states as well.
     */
    void reset();

    /**
     * @brief Returns the VirtualController for @p player.
     */
    VirtualController* getPlayerController(int player) const;

    static InputManager* getInstance();

signals:
    /**
     * @brief Emitted when player @p playerId becomes either activated (i.e. connects a controller)
     * or deactivated (i.e. disconnects their controller).
     * @param playerId Id of player who has become activated
     * @param status @c true if @p playerId has connected
     */
    void playerActivationChanged(int playerId, bool status);

protected:
    // Maps playerId to deviceId
    QMap<int, int> mPlayerToDeviceMap;
    // Reverse map from deviceId to playerId
    QMap<int, int> mDeviceToPlayerMap;
    // Lowest available player index which has yet to be assigned
    int mMinAvailablePlayerId = 0;

private:
    /**
     * Adds a new player for the specified deviceId.
     */
    void addPlayerController(int deviceId);
    /**
     * Removes a player with the specified deviceId.
     */
    void removePlayerController(int deviceId);

    /**
     * Listener for changes in device connection status sent by the ControllerManager.
     */
    virtual void onDeviceConnectionChanged(int deviceId, bool isConnected) override;


    static InputManager* mInstance;

};
Q_DECLARE_METATYPE(InputManager*)
#endif // INPUTMANAGER_H
