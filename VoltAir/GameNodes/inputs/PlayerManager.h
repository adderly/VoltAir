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

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <GameInput/ControllerManager.h>
#include <Engine/InputManager.h>
#include <QList>
#include <QObject>
#include <QMap>

class VirtualController;

/**
 * @brief Manages a mapping from unique player identifiers to different controllers that connect and
 * disconnect throughout the game.
 */
class PlayerManager : public InputManager {
    Q_OBJECT
public:
    /**
     * @brief Different actions or expressions of intent a player can make in the game.
     * @note PlayerAction%s are not necessarily distinctive amongst themselves, but rather represent
     * the full set of actions a player can express using different types of controllers supported
     * by PlayerManager.
     */
    enum PlayerAction {
        /**
         * @brief Unidirectional action for movement to the left.
         */
        LEFT,
        /**
         * @brief Unidirectional action for movement to the right.
         */
        RIGHT,
        /**
         * @brief Bidirectional action for movement left or right (i.e. horizontal).
         */
        HORIZONTAL,
        /**
         * @brief Action for magnetizing.
         */
        MAGNETIZE
    };

    /**
     * @brief Constructs a PlayerManager.
     * @param parent Parent object
     */
    explicit PlayerManager(QObject* parent = nullptr);

    virtual ~PlayerManager();

    virtual void init() override;

    /**
     * @brief Returns the horizontal movement value <tt>[-1.0f, 1.0f]</tt> for @p player.
     */
    float getHorizontal(int player) const;
    /**
     * @brief Returns whether or not the the @p player has magnetism activated.
     */
    bool isMagnetized(int player) const;

    /**
     * @brief Returns the global singleton instance of the PlayerManager, creating it if necessary.
     */
    static PlayerManager* getInstance();

private:

    // Singleton instance of the player manager
    static PlayerManager* sInstance;
};
Q_DECLARE_METATYPE(PlayerManager*)

#endif // PLAYERMANAGER_H
