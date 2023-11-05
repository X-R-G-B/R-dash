//
// Created by 33627 on 11/4/2023.
//

#include "B-luga-graphics/GraphicsSystems.hpp"
#include "B-luga/Json.hpp"
#include "B-luga/Registry.hpp"
#include "parallax.hpp"

namespace Parallax {

    static void initParallaxEntity(nlohmann::basic_json<> &elem, const int maxOffsideParallax = 0)
    {
        std::size_t id = Registry::getInstance().addEntity();
        auto parralax  = Raylib::Sprite::fromFile(
                Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
                Json::getInstance().getDataFromJson<float>(elem, "width"),
                Json::getInstance().getDataFromJson<float>(elem, "height"),
                id);

        Types::Position position = Json::getInstance().getDataFromJson<Types::Position>(elem, "position");
        Types::Velocity velocity = Json::getInstance().getDataFromJson<Types::Velocity>(elem, "velocity");

        if (Json::getInstance().isDataExist(elem, "rect")) {
            Types::Rect rect = Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect");
            Registry::getInstance().getComponents<Types::Rect>().insertBack((rect));
        }

        if (maxOffsideParallax > 0) {
            Maths::addNormalIntToDecimalInt(position.x, maxOffsideParallax);
        }
        Types::Parallax inst = {
                Maths::intToFloatConservingDecimals(position.x),
                Maths::intToFloatConservingDecimals(position.y)};
        Types::NoRemoveOutside noRem;

        Registry::getInstance().getComponents<Types::NoRemoveOutside>().insertBack(noRem);
        Registry::getInstance().getComponents<Raylib::SpriteShared>().insertBack(parralax);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<Types::Parallax>().insertBack(inst);
        Registry::getInstance().setToBackLayers(id);
    }

    void createParallax(const std::string &path)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Json::getInstance().registerJsonFile(path);
        std::vector<nlohmann::basic_json<>> parallaxData = Json::getInstance().getDataByJsonType(path, "parallax");

        for (auto &elem : parallaxData) {
            initParallaxEntity(elem);
            if (Json::getInstance().isDataExist(elem, "copy")
                && Json::getInstance().getDataFromJson<bool>(elem, "copy") == true) {
                Logger::info("copy");
                initParallaxEntity(elem, 100);
            }
        }
    }
} // namespace Parallax
