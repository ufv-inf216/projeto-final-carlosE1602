//
// Created by Marcelo on 29/11/2023.
//

#include <random>
#include "Ball.h"
#include "Player.h"

#define FLOOR_HEIGHT 60.0f
#define GRAVITY 9.8f
#define FRICTION 0.999f

BallCharacterHandler::BallCharacterHandler() {

}

const std::vector<int> BallCharacterHandler::GetStateArray(BallCharacter character, BallState state) {
    switch (character) {
        case BallCharacter::Default:
            switch (state) {
                case BallState::MoveToRight:
                    return {0,1,2,3,4,5,6,7};
                case BallState::MoveToLeft:
                    return {7,6,5,4,3,2,1,0};
                case BallState::Idle:
                    return {0};
            }
    }

    // Default to an empty array if no matching case is found
    return {};
}

const std::string BallCharacterHandler::GetCharacterSpriteSheetPath(BallCharacter character) {
    switch (character) {
        case BallCharacter::Default:
            return "../Assets/Sprites/ball/ball.png";
    }

    // Default to an empty string if no matching case is found
    return "";
}

const std::string BallCharacterHandler::GetCharacterSpriteSheetJSON(BallCharacter character) {
    switch (character) {
        case BallCharacter::Default:
            return "../Assets/Sprites/ball/ball.json";
    }

    return "";
}


Ball::Ball(Game *game, BallCharacter BallCharacter, Vector2 position,  float forwardSpeed, Direction mDirection)
        : Actor(game),
          mBallCharacter(BallCharacter),
          mForwardSpeed(forwardSpeed),
          mIsMoving(false),
          mDirection(Direction::None),
          mRestitution(0.8f),
          mShotSpeed(0.0f)
          {

    mRigidBodyComponent = new RigidBodyComponent(this, 2.0f, FRICTION);
    mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, mForwardSpeed));

    const float width = 64.0f;
    const float height = 64.0f;
    mColliderComponent = new AABBColliderComponent(this, 0, 0, width, height, ColliderLayer::Ball);

    mDrawPolygonComponent = new DrawPolygonComponent(this, width, height);

    mCharacter = new BallCharacterHandler();

    mDrawComponent = new DrawAnimatedComponent(this, mCharacter->GetCharacterSpriteSheetPath(mBallCharacter), mCharacter->GetCharacterSpriteSheetJSON(mBallCharacter));

    mDrawComponent->AddAnimation("moveToLeft", mCharacter->GetStateArray(mBallCharacter, BallState::MoveToLeft));
    mDrawComponent->AddAnimation("moveToRight", mCharacter->GetStateArray(mBallCharacter, BallState::MoveToRight));
    mDrawComponent->AddAnimation("idle", mCharacter->GetStateArray(mBallCharacter, BallState::Idle));
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Ball::OnProcessInput(const uint8_t *state) {

}

void Ball::OnUpdate(float deltaTime) {
    auto ballVelocity = mRigidBodyComponent->GetVelocity();
    mRigidBodyComponent->SetVelocity(ballVelocity * FRICTION);

    if (ballVelocity.x != 0 || ballVelocity.y != 0) {
        if (ballVelocity.x > 0) {
            mDrawComponent->SetAnimation("moveToLeft");
        } else {
            mDrawComponent->SetAnimation("moveToRight");
        }
        mIsMoving = true;


        float speedMagnitude = ballVelocity.Length();


        float speedScale = 0.1f;
        float adjustedFPS = 5.0f + speedMagnitude * speedScale;

        mDrawComponent->SetAnimFPS(adjustedFPS);
    } else {
        mIsMoving = false;
        mDrawComponent->SetAnimFPS(10.0f);  // Padrão quando não está se movendo
    }

    mRigidBodyComponent->ApplyForce(Vector2(0.0f, GRAVITY));

    if (mRigidBodyComponent->GetOwner()->GetPosition().y + mColliderComponent->GetHeight() > mGame->GetWindowHeight() - FLOOR_HEIGHT) {
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x, -mRigidBodyComponent->GetVelocity().y * mRestitution));
    }

    if((mRigidBodyComponent->GetOwner()->GetPosition().y > GetGame()->GetWindowHeight() - 520) &&
            (mRigidBodyComponent->GetOwner()->GetPosition().x < 307 || mRigidBodyComponent->GetOwner()->GetPosition().x > GetGame()->GetWindowWidth() - 307)
    ) {
        printf("Gool");
        mRigidBodyComponent->GetOwner()->SetPosition(mRigidBodyComponent->GetOwner()->GetPosition());
    }

    if (mRigidBodyComponent->GetOwner()->GetPosition().x < mGame->GetCameraPos().x)
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mGame->GetCameraPos().x, mRigidBodyComponent->GetOwner()->GetPosition().y));

    if (mRigidBodyComponent->GetOwner()->GetPosition().x + mColliderComponent->GetWidth()> mGame->GetWindowWidth())
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mGame->GetWindowWidth() - mColliderComponent->GetWidth(), mRigidBodyComponent->GetOwner()->GetPosition().y));

    if (mRigidBodyComponent->GetOwner()->GetPosition().y + mColliderComponent->GetHeight() > mGame->GetWindowHeight() - FLOOR_HEIGHT){
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mRigidBodyComponent->GetOwner()->GetPosition().x, mGame->GetWindowHeight() - mColliderComponent->GetHeight() - FLOOR_HEIGHT));
    }

    ManageAnimations();
}

void Ball::ManageAnimations() {
    mDrawComponent->SetIsPaused(false);

    if(mIsMoving && mDirection == Direction::Right) {
        mDrawComponent->SetAnimation("moveToRight");
    }
    else if(mIsMoving && mDirection == Direction::Left) {
        mDrawComponent->SetAnimation("moveToLeft");
    }
    else {
        mDrawComponent->SetAnimation("idle");
    }
}

void Ball::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap> &responses) {
    for (auto &response : responses) {

        Player* lastPlayer = dynamic_cast<Player *>(GetGame()->GetPlayers().back());
        Player* firstPlayer = dynamic_cast<Player *>(GetGame()->GetPlayers().front());

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist;
        int randomNumber = 1;
        if (firstPlayer->IsHeader() || lastPlayer->IsHeader() && mRigidBodyComponent->GetOwner()->GetPosition().y < 300){
            dist = std::uniform_int_distribution<int>(3,20);
            randomNumber = dist(mt);

        } else if (firstPlayer->IsShot() || lastPlayer->IsShot()) {
            dist = std::uniform_int_distribution<int>(2,10);
            randomNumber = dist(mt);
        } else {
            dist = std::uniform_int_distribution<int>(1,1);
            randomNumber = 1;
        }

        Vector2 newVelocity;
        switch (response.first) {
            case CollisionSide::Right:
                newVelocity = Vector2(-mForwardSpeed * randomNumber, mRigidBodyComponent->GetVelocity().y * randomNumber);
                mDirection = Direction::Left;
                mDrawComponent->SetAnimation("moveToLeft");
                break;
            case CollisionSide::Left:
                mDirection = Direction::Right;
                mDrawComponent->SetAnimation("moveToRight");
                newVelocity = Vector2(mForwardSpeed * randomNumber, mRigidBodyComponent->GetVelocity().y * randomNumber);
                break;
            case CollisionSide::Down:
                newVelocity = Vector2(mForwardSpeed * randomNumber, -mRigidBodyComponent->GetVelocity().y * randomNumber);
                break;
            case CollisionSide::Top:
                newVelocity = Vector2(mForwardSpeed * randomNumber, mRigidBodyComponent->GetVelocity().y * randomNumber);
                break;
        }
        mRigidBodyComponent->SetVelocity(newVelocity);
    }
}

void Ball::Kill() {

}