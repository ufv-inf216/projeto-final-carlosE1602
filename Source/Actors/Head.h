#ifndef JOGO_DA_CABECA_HEAD_H
#define JOGO_DA_CABECA_HEAD_H

#include "../Game.h"
#include <string>

enum HeadCharacter {
    ronaldo,
    messi
};

enum HeadCharacterState {
    Idle,
    Move,
    Jump,
    Shot,
    Header,
    Dead,
};

class HeadCharacterHandler {
    public:
        HeadCharacterHandler();

        const std::vector<int> GetStateArray(HeadCharacter character, HeadCharacterState state);
        const std::string GetCharacterSpriteSheetPath(HeadCharacter character);
        const std::string GetCharacterSpriteSheetJSON(HeadCharacter character);
    private:
};


#endif //JOGO_DA_CABECA_HEAD_H
