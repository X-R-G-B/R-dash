#include "B-luga/Registry.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Logger.hpp"
#include "B-luga-graphics/GraphicsSystems.hpp"
#include "GameSystems.hpp"
#include "CustomTypes.hpp"

namespace GameSystems {
    void gravity(std::size_t, std::size_t)
    {
        auto &players = Registry::getInstance().getComponents<Types::Player>();
        auto &vel = Registry::getInstance().getComponents<Types::Velocity>();
        auto ids = Registry::getInstance().getEntitiesByComponents({typeid(Types::Player), typeid(Types::Velocity)});
        static float incrementor = 0;
        static float decrementor = 1;

        for (auto id : ids) {
            Logger::info("gravity player act : " + std::to_string(players[id].action));
            if (players[id].action == Types::Action::TOJUMP) {
                incrementor = 20;
                vel[id].speedY += incrementor;
                incrementor -= decrementor;
                Logger::info("TOJUMP TO JUMP");
                players[id].action = Types::Action::JUMP;
            } else if (players[id].action == Types::Action::TOFALL) {
                incrementor = 0;
                vel[id].speedY += incrementor;
                incrementor -= decrementor;
                players[id].action = Types::Action::FALL;
            } else if (players[id].action == Types::Action::JUMP) {
                vel[id].speedY -= incrementor;
                incrementor -= decrementor;
                if (incrementor <= 0) {
                    players[id].action = Types::Action::TOFALL;
                }
            } else if (players[id].action == Types::Action::FALL) {
                vel[id].speedY -= incrementor;
                incrementor -= decrementor;
            }
            if (vel[id].speedY > 150) {
                vel[id].speedY = 150;
            }
        }
    }

    void jump(std::size_t, std::size_t)
    {
        auto &players = Registry::getInstance().getComponents<Types::Player>();

        if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_SPACE)) {
            for (auto &player : players) {
                Logger::info("jump player act : " + std::to_string(player.action));
                if (player.action == Types::Action::SLIDE) {
                    Logger::info("TOJUMP");
                    player.action = Types::Action::TOJUMP;
                }
            }
        }
    }

    static bool checkBlockCol(std::size_t playerId)
    {
        auto blocks = Registry::getInstance().getComponents<Types::Block>();
        auto pos = Registry::getInstance().getComponents<Types::Position>();
        auto collisionRect = Registry::getInstance().getComponents<Types::CollisionRect>();
        Types::Position realPosFstEntity   = {pos[playerId].x + collisionRect[playerId].offsetX, pos[playerId].y + collisionRect[playerId].offsetY};

        if (pos[playerId].y >= 9000) {
            return true;
        }
        auto ids = Registry::getInstance().getEntitiesByComponents({typeid(Types::Block), typeid(Types::Position), typeid(Types::CollisionRect)});
        for (auto id : ids) {
            Types::Position realPosSndEntity = {pos[id].x + collisionRect[id].offsetX, pos[id].y + collisionRect[id].offsetY};
            if (Maths::intToFloatConservingDecimals(realPosFstEntity.x)
                < (Maths::intToFloatConservingDecimals(realPosSndEntity.x)
                   + Maths::intToFloatConservingDecimals(collisionRect[id].width))
                && (Maths::intToFloatConservingDecimals(realPosFstEntity.x)
                    + Maths::intToFloatConservingDecimals(collisionRect[playerId].width))
                   > Maths::intToFloatConservingDecimals(realPosSndEntity.x)
                && Maths::intToFloatConservingDecimals(realPosFstEntity.y)
                   < (Maths::intToFloatConservingDecimals(realPosSndEntity.y)
                      + Maths::intToFloatConservingDecimals(collisionRect[id].height))
                && (Maths::intToFloatConservingDecimals(realPosFstEntity.y)
                    + Maths::intToFloatConservingDecimals(collisionRect[playerId].height))
                   > Maths::intToFloatConservingDecimals(realPosSndEntity.y)) {
                return true;
            }
        }
        return false;
    }

    void applyGravity(std::size_t, std::size_t)
    {
        auto &players = Registry::getInstance().getComponents<Types::Player>();
        auto &vel = Registry::getInstance().getComponents<Types::Velocity>();
        auto ids = players.getExistingsId();

        for (auto id : ids) {
            Logger::info("apply gravity player act : " + std::to_string(players[id].action));
            auto &player = players[id];
            if (player.action == Types::Action::FALL && checkBlockCol(id)) {
                Logger::info("set to slide");
                player.action = Types::Action::SLIDE;
                vel[id].speedY = 0;
            } else if (player.action == Types::Action::SLIDE && checkBlockCol(id) == false) {
                Logger::info("set to toFall");
                player.action = Types::Action::TOFALL;
            }
        }
    }

    void createPlayer()
    {
        Registry::getInstance().addEntity();
        auto spriteDatas = Types::SpriteDatas("assets/sprites/player/cubes_1.png", 1000, 1000, LayerType::DEFAULTLAYER, 0);
        auto player = Types::Player(Types::Action::TOFALL);
        auto collisionRect = Types::CollisionRect(1000, 1000);
        auto pos = Types::Position(100, 700);
        auto vel = Types::Velocity(0, 50);
        auto hp = Types::Health(1);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
        Registry::getInstance().getComponents<Types::Player>().insertBack(player);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Position>().insertBack(pos);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(vel);
        Registry::getInstance().getComponents<Types::Health>().insertBack(hp);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getGameSystems()
    {
        return {
            gravity,
            jump,
            applyGravity
        };
    }
}
