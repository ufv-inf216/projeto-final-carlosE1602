// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "Actors/Actor.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Random.h"
#include "SDL_image.h"
#include "Components/DrawComponents/DrawSpriteComponent.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include "Actors/Player.h"
#include "Actors/Ground.h"
#include "Actors/Ball.h"
#include "Actors/Goal.h"

Game::Game(int windowWidth, int windowHeight)
    : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mUpdatingActors(false), mWindowWidth(windowWidth), mWindowHeight(windowHeight) {
}

bool Game::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Jogo da Cabeca", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_RESIZABLE);
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    Random::Init();

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    InitializeActors();

    return true;
}

void Game::InitializeActors() {
    LoadLevel();

    Vector2 player1Position = Vector2(500.0f, mWindowHeight - 32.0f - 330.0f);
    Vector2 player2Position = Vector2(mWindowWidth - 500.0f - 330.0f, mWindowHeight - 32.0f - 330.0f);
    Vector2 ballPosition = Vector2(mWindowWidth / 2.0f - 32.0f, mWindowHeight / 2.0f - 32.0f);

    mPlayer2 = new Player(this, player2Position, 2, HeadCharacter::messi);
    mPlayer2->SetPosition(player2Position);
    mPlayer1 = new Player(this, player1Position, 1, HeadCharacter::ronaldo);
    mPlayer1->SetPosition(player1Position);
    mBall = new Ball(this, BallCharacter::Default, ballPosition,  200.0f, Direction::None);
    mBall->SetPosition(ballPosition);
    mGoal1 = new Goal(this, GoalSide::Left);
    mGoal1->SetPosition(Vector2(0, 520));
    mGoal2 = new Goal(this, GoalSide::Right);
    mGoal2->SetPosition(Vector2(mWindowWidth-400, 520));
}

void Game::LoadLevel() {
    // Add background image located at Assets/Levels/Arena.png
    auto* background = new Actor(this);
    background->SetPosition(Vector2(0.0f, 0.0f));
    new DrawSpriteComponent(background, "../Assets/Sprites/Background.jpg", mWindowWidth, mWindowHeight, 0);

//    auto* goal1_r = new Actor(this);
//    goal1_r->SetPosition(Vector2(0.0f, mWindowHeight - 520.0f));
//    new DrawSpriteComponent(goal1_r, "../Assets/Sprites/gol/gol_r.png", 400.0f, 500.0f, 100);
    auto* goal2_r = new Actor(this);
    goal2_r->SetPosition(Vector2(400.0f, mWindowHeight - 520.0f));
    new DrawSpriteComponent(goal2_r, "../Assets/Sprites/gol/gol2_r.png", 93.0f, 500.0f,1);

//    auto* goal1_l = new Actor(this);
//    goal1_l->SetPosition(Vector2(mWindowWidth - 400.0f, mWindowHeight - 520.0f));
//    new DrawSpriteComponent(goal1_l, "../Assets/Sprites/gol/gol_l.png", 400.0f, 500.0f, 100);
    auto* goal2_l = new Actor(this);
    goal2_l->SetPosition(Vector2(mWindowWidth - 493.0f, mWindowHeight - 520.0f));
    new DrawSpriteComponent(goal2_l, "../Assets/Sprites/gol/gol2_l.png", 93.0f, 500.0f, 1);

//    float blockHeight = 50.0f;
//    auto* ground = new Ground(this, Vector2(0, mWindowHeight - blockHeight));
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            Quit();
            break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(nullptr);

    for (auto actor : mActors) {
        actor->ProcessInput(state);
    }
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    // Update camera position
    UpdateCamera();
}

void Game::UpdateCamera() {

}

void Game::UpdateActors(float deltaTime) {
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors) {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor *> deadActors;
    for (auto actor : mActors) {
        if (actor->GetState() == ActorState::Destroy) {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors) {
        delete actor;
    }
}

void Game::AddActor(Actor *actor) {
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    } else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor *actor) {
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

std::vector<Actor *> Game::GetPlayers() {
    std::vector<Actor *> players;

    players.emplace_back(mPlayer1);
    players.emplace_back(mPlayer2);

    return players;
}

std::vector<Actor *> Game::GetGoals() {
    std::vector<Actor *> goals;

    goals.emplace_back(mGoal1);
    goals.emplace_back(mGoal2);

    return goals;
}

void Game::AddDrawable(class DrawComponent *drawable) {
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(), [](DrawComponent *a, DrawComponent *b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveDrawable(class DrawComponent *drawable) {
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent *collider) {
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent *collider) {
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    mColliders.erase(iter);
}

void Game::GenerateOutput() {
    // Set draw color to black
    SDL_SetRenderDrawColor(mRenderer, 107, 140, 255, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    for (auto drawable : mDrawables) {
        if (drawable->IsVisible()) {
            drawable->Draw(mRenderer);
        }
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

SDL_Texture *Game::LoadTexture(const std::string &texturePath) {
    SDL_Surface *surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load texture file %s", texturePath.c_str());
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("Failed to convert surface to texture for %s", texturePath.c_str());
        return nullptr;
    }

    return texture;
}

void Game::Shutdown() {
    while (!mActors.empty()) {
        delete mActors.back();
    }

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
