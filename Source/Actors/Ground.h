#ifndef JOGO_DA_CABECA_GROUND_H
#define JOGO_DA_CABECA_GROUND_H

#pragma once
#include "Actor.h"


class Ground: public Actor {
public:
    explicit Ground(Game* game, Vector2 position = Vector2::Zero);

private:
    Vector2 mStartPosition;

    class DrawPolygonComponent* mDrawPolygonComponent;
    class AABBColliderComponent* mColliderComponent;
    class RigidBodyComponent* mRigidBodyComponent;
};


#endif //JOGO_DA_CABECA_GROUND_H
