
#include <fstream>
#include "B-luga/Json.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Registry.hpp"
#include "B-luga-graphics/Raylib/Graphics/Graphics.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "parallax.hpp"
#include "createMap.hpp"
#include "CustomTypes.hpp"
#include "GameSystems.hpp"

namespace Map {
    static void createElem(const std::string &path, char c, Types::Position pos)
    {
        std::string value(1, c);
        Registry::getInstance().addEntity();
        auto spritePath = Json::getInstance().getDataFromJson<std::string>(Json::getInstance().getDataByJsonType(path, value), "path");
        Types::SpriteDatas spriteDatas = {spritePath, 1000, 1000, LayerType::FRONTLAYER, 0};

        auto collisionRect = Types::CollisionRect{1000, 1000};
        auto type = Json::getInstance().getDataFromJson<std::string>(Json::getInstance().getDataByJsonType(path, value), "type");
        auto vel = Types::Velocity{-40, 0};
        auto noRem = Types::NoRemoveOutside();
        if (type == "block") {
            Types::Block block = {};
            Registry::getInstance().getComponents<Types::Block>().insertBack(block);
        } else if (type == "spike") {
            spriteDatas.width = 900;
            spriteDatas.height = 900;
            pos.y += 100;
            pos.x += 50;
            Types::Spike spike = {};
            Types::Damage damage = {1};
            Registry::getInstance().getComponents<Types::Spike>().insertBack(spike);
            Registry::getInstance().getComponents<Types::Damage>().insertBack(damage);
        } else if (type == "bumper") {
            Types::Bumper bumper = {};
           Registry::getInstance().getComponents<Types::Bumper>().insertBack(bumper);
        }
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(vel);
        Registry::getInstance().getComponents<Types::Position>().insertBack(pos);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::NoRemoveOutside>().insertBack(noRem);
    }

    void createMap(const std::string &path)
    {
        std::ifstream file(path);
        Raylib::Window::setWindowSize(800, 600);
        Logger::info("create map");
        auto pathSettings = "assets/maps/mapSettings.json";
        Json::getInstance().registerJsonFile(pathSettings);
        GameSystems::createPlayer();
        Parallax::createParallax("assets/maps/parallax/fst.json");
        Types::Position pos = {-1000, 9000};

        std::string line = "";
        std::vector<std::string> lines;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        for (auto it = lines.end(); it != lines.begin();) {
            it--;
            line = *it;
            Logger::info("line : " + line);
            for (auto c : line) {
                pos.x += 1000;
                if (c == '\n' || c == '\r' || c == ' ') {
                    continue;
                }
                createElem(pathSettings, c, pos);
            }
            pos.x = -1000;
            pos.y -= 1000;
        }
    }
} // namespace Map
