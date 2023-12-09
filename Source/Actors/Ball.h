//
// Created by Marcelo on 29/11/2023.
//

#ifndef JOGO_DA_CABECA_BALL_H
#define JOGO_DA_CABECA_BALL_H


#include <vector>
#include <string>
#include "../Game.h"
#include "Actor.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

enum Direction {
    Left,
    Right,
    None
};

enum class BallCharacter {
    Default // Add more characters if needed
};

enum class BallState {
    MoveToLeft,
    MoveToRight,
    Idle
};

class BallCharacterHandler {
public:
    BallCharacterHandler();

    const std::vector<int> GetStateArray(BallCharacter character, BallState state);
    const std::string GetCharacterSpriteSheetPath(BallCharacter character);
    const std::string GetCharacterSpriteSheetJSON(BallCharacter character);
private:


};

class Ball: public Actor {
public:
    explicit Ball(Game* game, BallCharacter BallCharacter, Vector2 position, float forwardSpeed = 2500.0f, Direction mDirection = Direction::None);

//    Ball(Game *game, BallCharacter BallCharacter, Vector2 position, float forwardSpeed, Direction mDirection);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    void Kill() override;

private:
    void ManageAnimations();

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
    class DrawPolygonComponent* mDrawPolygonComponent;

    float mForwardSpeed;
    Vector2 mDirectionVector;
    float mShotSpeed;
    bool mIsMoving;
    Direction mDirection;
    float mRestitution;
    BallCharacterHandler* mCharacter;
    BallCharacter mBallCharacter;
};







#endif //JOGO_DA_CABECA_BALL_H
