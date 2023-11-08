#include "B-luga/Logger.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga-physics/ECSSystems.hpp"
#include "B-luga-graphics/GraphicsSystems.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "B-luga/PluginHandler.hpp"
#include "CustomTypes.hpp"
#include "createMap.hpp"
#include "CustomTypes.hpp"
#include "GameSystems.hpp"

enum SystemManagers {
    ECS,
    GRAPHICS,
    GAME
};

static void callCreateMap(std::size_t /*event*/) {
    Map::createMap("assets/maps/obstacles/fst.txt");
}

static void checkEsc(std::size_t)
{
    if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_ESCAPE)) {
        Raylib::Window::closeWindow();
    }
}

int main()
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#else
    Logger::setLogLevel(LogLevel::Warn);
#endif

    int res;

    try {
        auto ecsPlugin = Systems::ECSPlugin();
        auto graphicsPlugin = Systems::GraphicsSystems::GraphicsPlugin();
        PluginHandler::addNewPlugin(ecsPlugin, SystemManagers::ECS);
        PluginHandler::addNewPlugin(graphicsPlugin, SystemManagers::GRAPHICS);
        Registry::getInstance().addEventCallback(Events::BEFORE_LOOP, callCreateMap);
        Systems::SystemManagersDirector::getInstance().addSystemManager(static_cast<std::size_t>(SystemManagers::GAME), GameSystems::getGameSystems());

        Registry::getInstance().addEventCallback(Events::START_LOOP, checkEsc);

        auto sceneManager = Scene::SceneManager::getInstance();
        sceneManager.setScenes({{SystemManagers::ECS, SystemManagers::GRAPHICS, SystemManagers::GAME}});
        res = sceneManager.run();
    } catch (const std::exception &e) {
        Logger::fatal(e.what());
        return 84;
    }
    return res;
}
