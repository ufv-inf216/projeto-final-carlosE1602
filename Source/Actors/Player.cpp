#include "Player.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Game.h"

#define FLOOR_HEIGHT 60.0f

Player::Player(Game *game, Vector2 position, int playerNumber, HeadCharacter HeadCharacter, float forwardSpeed, float jumpSpeed)
        : Actor(game),
          mPlayerNumber(playerNumber),
          mHeadCharacter(HeadCharacter),
          mForwardSpeed(forwardSpeed),
          mJumpSpeed(jumpSpeed),
          mIsDead(false),
          mIsJumping(false),
          mIsMoving(false),
          mIsShot(false),
          mIsHeader(false)
          {

    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0f);
    const float width = 330.0f;
    const float height = 330.0f;
    mColliderComponent = new AABBColliderComponent(this, 0, 0, width, height, ColliderLayer::Player);

    mDrawPolygonComponent = new DrawPolygonComponent(this, width, height);

    mRotation = mPlayerNumber == 1 ? Math::Pi : 0.0f;

    mCharacter = new HeadCharacterHandler();

    mDrawComponent = new DrawAnimatedComponent(this, mCharacter->GetCharacterSpriteSheetPath(mHeadCharacter), mCharacter->GetCharacterSpriteSheetJSON(mHeadCharacter));

    mDrawComponent->AddAnimation("idle", mCharacter->GetStateArray(mHeadCharacter, HeadCharacterState::Idle));
    mDrawComponent->AddAnimation("move", mCharacter->GetStateArray(mHeadCharacter, HeadCharacterState::Move));
    mDrawComponent->AddAnimation("jump", mCharacter->GetStateArray(mHeadCharacter, HeadCharacterState::Jump));
    mDrawComponent->AddAnimation("shot", mCharacter->GetStateArray(mHeadCharacter, HeadCharacterState::Shot));
    mDrawComponent->AddAnimation("header", mCharacter->GetStateArray(mHeadCharacter, HeadCharacterState::Header));
    mDrawComponent->AddAnimation("dead", mCharacter->GetStateArray(mHeadCharacter, HeadCharacterState::Dead));

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Player::OnProcessInput(const uint8_t *state) {
        if (mPlayerNumber == 1 && state[SDL_SCANCODE_D] || mPlayerNumber == 2 && state[SDL_SCANCODE_RIGHT]) {
            mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed, 0.0f));
            mIsMoving = true;
        }

        if (mPlayerNumber == 1 && state[SDL_SCANCODE_A] || mPlayerNumber == 2 && state[SDL_SCANCODE_LEFT]) {
            mRigidBodyComponent->ApplyForce(Vector2(-mForwardSpeed, 0.0f));
            mIsMoving = true;
        }

        if (mPlayerNumber == 1 && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_A] || mPlayerNumber == 2 && !state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]) {
            mIsMoving = false;
        }

        if (mPlayerNumber == 1 && state[SDL_SCANCODE_W] || mPlayerNumber == 2 && state[SDL_SCANCODE_UP]) {
                if (!mIsJumping) {

                    mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, mJumpSpeed));

                    mIsJumping = true;
                }
        }

        if (mPlayerNumber == 1 && state[SDL_SCANCODE_F] || mPlayerNumber == 2 && state[SDL_SCANCODE_RCTRL]) {
            mIsShot = true;
            mIsJumping = false;
            mIsMoving = false;
            mIsHeader = false;
        } else {
            mIsShot = false;
        }

        if (mPlayerNumber == 1 && state[SDL_SCANCODE_E] || mPlayerNumber == 2 && state[SDL_SCANCODE_RSHIFT]) {
            mIsHeader = true;
            mIsShot = false;
            mIsJumping = false;
            mIsMoving = false;
        } else {
            mIsHeader = false;
        }
}

void Player::OnUpdate(float deltaTime) {
    if (mRigidBodyComponent->GetOwner()->GetPosition().x < mGame->GetCameraPos().x)
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mGame->GetCameraPos().x, mRigidBodyComponent->GetOwner()->GetPosition().y));

    if (mRigidBodyComponent->GetOwner()->GetPosition().x + mColliderComponent->GetWidth()> mGame->GetWindowWidth())
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mGame->GetWindowWidth() - mColliderComponent->GetWidth(), mRigidBodyComponent->GetOwner()->GetPosition().y));

    if (mRigidBodyComponent->GetOwner()->GetPosition().y + mColliderComponent->GetHeight() > mGame->GetWindowHeight() - FLOOR_HEIGHT){
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mRigidBodyComponent->GetOwner()->GetPosition().x, mGame->GetWindowHeight() - mColliderComponent->GetHeight() - FLOOR_HEIGHT));
//        mIsOnGround = true;
        mIsJumping = false;
    }

    auto players = GetGame()->GetPlayers();
    if (players.front() == this && mRigidBodyComponent->GetOwner()->GetPosition().x > GetGame()->GetWindowWidth()/2 - 200) {
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mGame->GetWindowWidth()/2 - 200, mRigidBodyComponent->GetOwner()->GetPosition().y));
    }
    if (players.back() == this && mRigidBodyComponent->GetOwner()->GetPosition().x < GetGame()->GetWindowWidth()/2) {
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mGame->GetWindowWidth()/2, mRigidBodyComponent->GetOwner()->GetPosition().y));
    }

    ManageAnimations();
}

void Player::ManageAnimations() {
    mDrawComponent->SetIsPaused(false);

    if(mIsDead) {
        mDrawComponent->SetAnimation("dead");
    }
    else if(mIsMoving) {
        mDrawComponent->SetAnimation("move");
    }
    else if(mIsJumping) {
        mDrawComponent->SetAnimation("jump");
    }
    else if(mIsShot) {
        mDrawComponent->SetAnimation("shot");
    }
    else if(mIsHeader) {
        mDrawComponent->SetAnimation("header");
    }
    else {
        mDrawComponent->SetAnimation("idle");
    }

}



void Player::Kill() {
    mDrawComponent->SetAnimation("dead");
    mIsDead = true;
    mColliderComponent->SetEnabled(false);
}

void Player::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap> &responses) {
    for (auto &response : responses) {
    }
}