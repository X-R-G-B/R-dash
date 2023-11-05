#pragma once

namespace GameSystems {
    std::vector<std::function<void(std::size_t, std::size_t)>> getGameSystems();
    void checkDie(std::size_t, std::size_t);
    void applyGravity(std::size_t, std::size_t);
    void createPlayer();
}
