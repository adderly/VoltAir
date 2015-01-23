import QtQuick 2.2
import VoltAir 2.0

/**
*   Template ui internal for new game.

* @ingroup QQuickItem
* @brief The top-level UI object.
*
* The UI root serves as a manager of all possible Ui viewing @c state%s.
*
*/
UiInternal
{
        id:root
        Rectangle {
            //anchors.fill: parent
            width: 100
            height: 62
            //color:"red"
        }

        /**
         * @brief First state that the Ui should transition to.
         */
        property string initialState: "SPLASH_SCREEN"
        /**
         * @brief Default transition duration time for changes in Ui's @c state.
         */
        property int transitionDuration: 100
        /**
         * @brief type:SinglePlayerHud Reference to the single player HUD.
         */
        property alias singlePlayerHud: singlePlayerHud

       /**
        *   @brief property
        */
        menuBGMTrack: "music/music_loop_menu.mp3"


        /**
         * @brief Called by Game when attempting to start the Ui.
         */
        function tryStart() {
            if (state == "STARTING") {
                state = initialState
            }
        }

        /**
         * @brief Passes information about the current level to the Ui.
         * @param type:string levelName Name of the level
         * @param type:int completionThreshold How many points are needed to complete the level
         * @param type:int activationThreshold How many points are needed to pass the level
         */
        function onLevelChanged(levelName, completionThreshold, activationThreshold) {
            d.currentLevelName = levelName
            if (d.isMultiplayerProgression()) {
                multiPlayerHud.onLevelChanged(completionThreshold)
                multiPlayerScoringScreen.completionThreshold = completionThreshold
            } else {
                singlePlayerHud.onLevelChanged(levelName, completionThreshold, activationThreshold)
            }
            root.state = "GAMEPLAY"
        }

        /**
         * @brief Notifies the Ui when the level is completed.
         * @param type:bool gameCompleted Whether or not the game was completed
         * @param type:int oneStarThreshold How many points are needed to win one star
         * @param type:int twoStarThreshold How many points are needed to win two stars
         * @param type:int threeStarThreshold How many points are needed to win three stars
         * @param type:int fourStarThreshold How many points are needed to win four stars
         * @param type:int score Score achieved by the player
         */
        function onLevelCompleted(gameCompleted, oneStarThreshold, twoStarThreshold, threeStarThreshold,
                fourStarThreshold, score) {
            if (d.isMultiplayerProgression()) {
                multiPlayerScoringScreen.gameCompleted = gameCompleted
            } else {
                singlePlayerScoringScreen.gameCompleted = gameCompleted
                singlePlayerScoringScreen.oneStarThreshold = oneStarThreshold
                singlePlayerScoringScreen.twoStarThreshold = twoStarThreshold
                singlePlayerScoringScreen.threeStarThreshold = threeStarThreshold
                singlePlayerScoringScreen.fourStarThreshold = fourStarThreshold
                singlePlayerScoringScreen.score = score
            }
            root.state = "SCORING_SCREEN"
        }

        /**
         * @brief Notifies the Ui when a player's activation changes.
         * @param type:int player ID of player that had their activation change
         * @param type:bool active Active state to set the player to
         */
        function onPlayerActivationChanged(player, active) {
            // We only care about dynamic activation in multiplayer.
            if (d.isMultiplayerProgression()) {
                multiPlayerHud.setPlayerActivation(player, active)
                multiPlayerScoringScreen.setPlayerActivation(player, active)
            }
        }

        /**
         * @brief Notifies the Ui when a player's score changes.
         * @param type:int player ID of player whose score changed
         * @param type:int score Updated player score
         */
        function onPlayerScoreChanged(player, score) {
            if (d.isMultiplayerProgression()) {
                multiPlayerHud.onPlayerScoreChanged(player, score)
                multiPlayerScoringScreen.onPlayerScoreChanged(player, score)
            } else {
                singlePlayerHud.onPlayerScoreChanged(score)
            }
        }

        /**
         * @brief Notifies the Ui when the player data profile has been updated with new data.
         */
        function onPlayerDataChanged() {
            levelSelectionMenu.onPlayerDataChanged()
        }

        /**
         * @brief Notifies the Ui when the device's @c onPause lifecycle event occurs.
         */
        function onDevicePause() {
            if (state == "GAMEPLAY") {
                state = "PAUSE_MENU"
            }
        }

        anchors.fill: parent

        state: "STARTING"

        states: [
            // Game just started up.
            State { name: "STARTING" },
            // The splash screen is active.
            State { name: "SPLASH_SCREEN" },
            // Menu to choose between single- and multi-player active.
            State { name: "LEVEL_SELECTION_MENU" },
            // The Ui menus are inactive.
            State { name: "GAMEPLAY" },
            // Showing the score of the just-completed level.
            State { name: "SCORING_SCREEN" },
            // The final reward screen / movie is showing.
            State { name: "REWARD_SCREEN" },
            // Pause Menu is active.
            State { name: "PAUSE_MENU" },
            // Loading screen to give feedback while loading.
            State { name: "LOADING_SCREEN" },
            // Quit Dialog is active.
            State { name: "QUIT_DIALOG" },
            // In the process of shutting down the app.
            State { name: "QUITTING" }
        ]

}


