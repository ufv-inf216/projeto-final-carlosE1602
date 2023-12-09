#ifndef JOGO_DA_CABECA_PLAYER_H
#define JOGO_DA_CABECA_PLAYER_H

#pragma once
#include "Actor.h"
#include "Head.h"

class Player: public Actor {
public:
    explicit Player(Game* game, Vector2 position, int playerNumber, HeadCharacter HeadCharacter, float forwardSpeed = 2500.0f, float jumpSpeed = -5000.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;

    bool IsShot() const { return mIsShot; }
    bool IsHeader() const { return mIsHeader; }

    void Kill() override;

private:
    void ManageAnimations();

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
    class DrawPolygonComponent* mDrawPolygonComponent;

    float mForwardSpeed;
    float mJumpSpeed;
    bool mIsMoving;
    bool mIsDead;
    bool mIsJumping;
    bool mIsShot;
    bool mIsHeader;
    int mPlayerNumber;
    HeadCharacterHandler* mCharacter;
    HeadCharacter mHeadCharacter;
};


#endif //JOGO_DA_CABECA_PLAYER_H
