#pragma once

namespace Types {
    struct Block {
    };

    struct Spike {
    };

    struct Bumper {
    };

    enum Action {
        JUMP,
        TOJUMP,
        SLIDE,
        TOFALL,
        FALL
    };

    struct Player {
        enum Action action;
    };
}