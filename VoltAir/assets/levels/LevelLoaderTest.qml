import QtQuick 2.2
import VoltAir 1.0
import "../editor"

Level {
    cameraBoundary: Qt.rect(57.83268, -10, 50.3274, 10.8874)
    fileName: "LevelLoaderTest"
    jsonFilePath: Util.getPathToAsset("assets/levels/LevelLoaderTest.json")
    levelType: Level.RUBE_BOX2D_LEVEL
    mouseToInputArea:true
    fillCamera: true

    //jsonData:""
    parallaxOrigin.x: 15
    parallaxOrigin.y: -21.697891
    parallaxStrength.y: 1


    //all the bodies that come with actorProperty set to one of
    // these are automatically instantiated to these actors
    levelActorMapping:[
        {     "enemy":"qml/game/actors/Enemy.qml"      },
        {     "three":"qml/game/actors/Three.qml"      }
    ]


    LevelNode{
        type:LevelNode.ACTOR_COMPONENT_W_BODY
        targetName: "qml/game/actors/Enemy.qml"
        targetBody:"body5"
    }

    Actor{
        x: 1
        y: 1
        LevelNode{
            type:LevelNode.BODY_REFERENCE
            targetName: ""
            targetBody:"body7"
        }
    }



    Actor{
        id:enemy
        objectName: "actor1"

        FallowLogic{
            name:"assss"
            currentAI:true
            isDirectional:true
            target: actor0.body
            onDirectionChanged: {
//                console.log(actor1+" Changed Direction ")
            }
        }
        ActorProperties{
            id:props
        }
    }

    Actor{
        x: 1
        y: 1
        CircleBody{
            bodyType:Body.DynamicBody
            active:true
            radius:0.4
            categoryBits : GameHelper.ENEMY
            maskBits: GameHelper.collideWithAllBut(GameHelper.ENEMY)
        }

        AnimatedImageRenderer{
            jsonFilename: Util.getPathToAsset("assets/img/TestJsonArr.json")
            defaultClip: "tp_strip"
            useGameTime: false
            centered:true
            z:15
            stripOrientation : AnimatedImageRenderer.HORIZONTAL

            AnimatedImageClip{
                name:"multi"
                frameDelay: 0.2
                clipType: AnimatedImageClip.MULTI_FRAME

                frames:[
                    "Default.png",
                    "HelloWorld.png",
                    "Icon",
                    "Icon-Small@2x.png",
                    "Icon@2x.png",
//                    "Icon-Small-50.png",
//                    "Icon-72.png"

                ]
            }
            AnimatedImageClip{
                name:"tp_strip"
                frameDelay: 0.2
                clipType: AnimatedImageClip.SINGLE_FRAME_STRIP
                frameCount:4
                frameOffset:0
                stripFrameAmount: 4

                frames:[
                    "1197106712447482105johnny_automatic_walking.svg"

                ]
            }
            AnimatedImageClip{
                frameDelay: 0.1
                clipType: AnimatedImageClip.MULTI_FRAME

                frames:[
                    "Icon",
                    "Icon.png",
                    "FrameOne.png",
                    "FrameTwo.png",
                    "FrameThree.png",
                    "FrameFour.png",
                ]
            }

        }
    }

    Actor{
        id: actor382489
        objectName: "actor382489"
        rotation: -33.930023
        x: 1
        y: 1

        PolygonBody {
            id: pbody
            bodyType: Body.DynamicBody
            manualInitialization: true
            //vertices: [[-0.528791, -0.748077], [1.04714, -0.748077], [1.04714, -3.32038], [-0.528791, -3.32038]]
        }
    }
    Actor{
        id:actor1
        actorType:Actor.GenericActor
        x:1
        y:4
        CircleBody {
            id: body
            bodyType: Body.DynamicBody
            manualInitialization: true
            initialX: -1000.5
            initialY: -1000.5
            radius: 1.0
            density: 2.0
            friction: 0.9
            restitution: 0.1
            linearDamping: 0.2
            angularDamping: 0.20

            magnetic: false
            magneticStrength: 0.25

            onContactBegun: {
//                console.error("ASDASDADSD")
            }
        }

        FallowLogic{
            name:"as"
            currentAI:true
            target: actor0.body
        }
    }


    ParticleLayer {
        density: 20
        gravityScale: 0.65
        id: particleLayer0
        radius: 0.075
    }
    ParticleLayer {
        density: 0.2
        gravityScale: 1.5
        id: particleLayer1
    }
    ParticleLayer {
        id: particleLayer2
    }
    ParticleLayer {
        id: particleLayer3
    }
}
