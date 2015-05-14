
#include <Engine/utils/Util.h>
#include <GameInput/JoystickAxisScheme.h>
#include <GameInput/KeyScheme.h>
#include <GameInput/TriggerAxisScheme.h>
#include <GameInput/VirtualController.h>
#include <GameInput/routers/GamepadRouter.h>
#include <GameInput/routers/KeyboardRouter.h>
#include <GameInput/routers/TouchNavigationRouter.h>
#include <algorithm>
#include "InputManager.h"

InputManager* InputManager::mInstance = nullptr;

InputManager* InputManager::getInstance(){
    if(!mInstance){
        new InputManager();
    }
    return mInstance;
}

InputManager::InputManager(QObject* parent):QObject(parent),
            ControllerManager::DeviceConnectionListener()
{
    ControllerManager::getInstance()->registerDeviceConnectionListener(this);
    init();
    mInstance = this;
}

InputManager::~InputManager()
{
    // Need to remove the dangling reference to this class since it is being destroyed
    ControllerManager::getInstance()->unregisterDeviceConnectionListener(this);
}

void InputManager::init()
{
}

void InputManager::reset() {
    // Calling this will notify us below of devices being disconnected.
    ControllerManager::getInstance()->reset();
}

VirtualController* InputManager::getPlayerController(int player) const {
    if (!mPlayerToDeviceMap.contains(player)) {
        return nullptr;
    }
    return ControllerManager::getInstance()->getController(mPlayerToDeviceMap[player]);
}


void InputManager::onDeviceConnectionChanged(int deviceId, bool isConnected) {
    if (isConnected) {
        addPlayerController(deviceId);
    } else {
        removePlayerController(deviceId);
    }
}


void InputManager::addPlayerController(int deviceId) {
    int playerId = mMinAvailablePlayerId;
    mPlayerToDeviceMap[playerId] = deviceId;
    mDeviceToPlayerMap[deviceId] = playerId;

    // Increment the minimum available player id until you find an available id
    // This must now be the minimum available player id since by definition the
    // starting id was the minimum before.
    while (mPlayerToDeviceMap.contains(++mMinAvailablePlayerId)) {
    }

    emit playerActivationChanged(playerId, true);
}

void InputManager::removePlayerController(int deviceId) {
    if (!mDeviceToPlayerMap.contains(deviceId)) {
        return;
    }

    int playerId = mDeviceToPlayerMap[deviceId];
    mPlayerToDeviceMap.remove(playerId);
    mDeviceToPlayerMap.remove(deviceId);

    // Check if this id becoming available is now the minimum id available
    mMinAvailablePlayerId = std::min(mMinAvailablePlayerId, playerId);

    emit playerActivationChanged(playerId, false);
}
