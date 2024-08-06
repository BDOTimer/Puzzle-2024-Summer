#ifndef CONFIG_H
#define CONFIG_H
///----------------------------------------------------------------------------|
/// "config.h"
///----------------------------------------------------------------------------:

#include "core.h"

///----------------------------------------------------------------------------|
/// Конфиг.
///----------------------------------------------------------------------------:
///---------------------|
/// Общая часть.        |
///---------------------:
struct  Config
{
    sf::Vector2i winsize{700 * 1366 / 768, 700};

    sf::View* view[2];

    sf::View& getView_World(){ return *view[0]; }
    sf::View& getView_GUI  (){ return *view[1]; }

    static inline double deltaTime = 0.0;

    static inline std::string path2res{"../../../../Resources/"};

    static std::string getFileName(unsigned n)
    {   return Config::path2res + filename [n];
    }

private:
    static inline const char* filename[]
    {   "01.jpg",
        "02.jpg",
        "photo_2024-08-04_20-24-52.jpg"
    };
};

///---------------------|
/// Игровая часть.      |
///---------------------:
struct  ConfigGame : Config
{
    ManagerScenes managerScenes;

    std::vector<std::unique_ptr<Obj::IObject>> objs;

    void init()
    {
        ///------------------------------|
        /// Получаем набор объектов.     |
        ///------------------------------:
        extern std::vector<std::unique_ptr<Obj::IObject>> createObjs();
        objs =                                            createObjs();
    }

    static inline ConfigGame* p = nullptr;
};

#endif // CONFIG_H
