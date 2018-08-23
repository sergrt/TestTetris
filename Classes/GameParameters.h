#pragma once

namespace Tetris {
    namespace Field {
        constexpr int width = 12;
        constexpr int topSpace = 3; // rows without left and rigth borders to allow figure to be spawn almost outside cup
        constexpr int height = 21 + topSpace;
    }
    namespace Figure {
        constexpr int patternSize = 4; // figure matrix is square
    }
    namespace Sprites {
        constexpr int brickSpriteSize = 24;
    }

    namespace Gameplay {
        constexpr float gravityTimer = 0.8f;
        constexpr float transitTime = 0.3f;
        constexpr float gravityTime = gravityTimer / 2.0f;
        constexpr float gravityTimerTolerance = gravityTimer * 1.5f; // max gravity timer to allow smooth figure manipulation by keyboard
        constexpr float keyHoldRepeatInterval = 0.15f;
        //constexpr float transitDistance = 20.0f;
    }
}
