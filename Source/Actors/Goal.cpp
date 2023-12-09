//
// Created by Marcelo on 03/12/2023.
//

#include "Goal.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Game.h"

Goal::Goal(Game *game, GoalSide side)
        : Actor(game)
    {
    float width = 500.0f;
    float height = 500.0f;
    mRigidBodyComponent = new RigidBodyComponent(this);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, width, height, ColliderLayer::Goal);
    mDrawPolygonComponent = new DrawPolygonComponent(this, width, height);
    if(side == GoalSide::Left)
        mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/gol/gol_r.png", "../Assets/Sprites/gol/gol.json");
    else
        mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/gol/gol_l.png", "../Assets/Sprites/gol/gol.json");

    mDrawComponent->AddAnimation("default", std::vector<int>{0});
    mDrawComponent->SetAnimation("default");
    mDrawComponent->SetAnimFPS(10.0f);

    }

void Goal::OnProcessInput(const uint8_t *state) {
}

void Goal::OnUpdate(float deltaTime) {
    if(mRigidBodyComponent->GetOwner()->GetPosition().y > GetGame()->GetWindowHeight() - 520) {
        mRigidBodyComponent->GetOwner()->SetPosition(Vector2(mRigidBodyComponent->GetOwner()->GetPosition().x, GetGame()->GetWindowHeight() - 520));
    }

    ManageAnimations();
}

void Goal::ManageAnimations() {
    mDrawComponent->SetIsPaused(false);
    mDrawComponent->SetAnimation("default");
}

void Goal::Kill() {
}

void Goal::OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap> &responses) {
    for (auto &response : responses) {
    }
}