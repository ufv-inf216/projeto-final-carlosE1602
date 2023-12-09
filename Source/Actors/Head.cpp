#include "Head.h"

HeadCharacterHandler::HeadCharacterHandler() {

}

const std::vector<int> HeadCharacterHandler::GetStateArray(HeadCharacter character, HeadCharacterState state) {
    switch (character) {
        case HeadCharacter::ronaldo:
            switch (state) {
                case HeadCharacterState::Idle:
                    return {0};
                case HeadCharacterState::Move:
                    return {0};
                case HeadCharacterState::Jump:
                    return {0};
                case HeadCharacterState::Shot:
                    return {1, 1};
                case HeadCharacterState::Header:
                    return {3, 3};
                case HeadCharacterState::Dead:
                    return {2,2,2};
            }
        case HeadCharacter::messi:
            switch (state) {
                case HeadCharacterState::Idle:
                    return {0};
                case HeadCharacterState::Move:
                    return {0};
                case HeadCharacterState::Jump:
                    return {0};
                case HeadCharacterState::Shot:
                    return {1, 1};
                case HeadCharacterState::Header:
                    return {3, 3};
                case HeadCharacterState::Dead:
                    return {2, 2, 2};
            }
    }
}

const std::string HeadCharacterHandler::GetCharacterSpriteSheetPath(HeadCharacter character) {
    switch (character) {
        case HeadCharacter::ronaldo:
            return "../Assets/Sprites/ronaldo/ronaldo.png";
        case HeadCharacter::messi:
            return "../Assets/Sprites/messi/messi1.png";
    }
}

const std::string HeadCharacterHandler::GetCharacterSpriteSheetJSON(HeadCharacter character) {
    switch (character) {
        case HeadCharacter::ronaldo:
            return "../Assets/Sprites/ronaldo/ronaldo.json";
        case HeadCharacter::messi:
            return "../Assets/Sprites/messi/messi1.json";
    }
}
