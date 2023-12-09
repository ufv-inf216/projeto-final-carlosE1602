//
// Created by Marcelo on 03/12/2023.
//

#ifndef JOGO_DA_CABECA_GOAL_H
#define JOGO_DA_CABECA_GOAL_H


#include "Actor.h"

enum class GoalSide {
    Right,
    Left
};

class Goal : public Actor {
public:
    explicit Goal(Game* game, GoalSide side);
    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnCollision(std::unordered_map<CollisionSide, AABBColliderComponent::Overlap>& responses) override;
    void Kill() override;


private:
    void ManageAnimations();

    class RigidBodyComponent* mRigidBodyComponent;
    class AABBColliderComponent* mColliderComponent;
    class DrawPolygonComponent* mDrawPolygonComponent;
    class DrawAnimatedComponent* mDrawComponent;
};


#endif //JOGO_DA_CABECA_GOAL_H
