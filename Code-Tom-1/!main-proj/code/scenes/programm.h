#ifndef PROGRAMM_H
#define PROGRAMM_H
///----------------------------------------------------------------------------|
/// "programm.h"
///----------------------------------------------------------------------------:

#include "core.h"
#include "config.h"
#include "render.h"


///----------------------------------------------------------------------------|
/// Programm.
///----------------------------------------------------------------------------:
struct  Programm
{       Programm()
        {
            ///------------------------------|
            /// Инициализация конфига.       |
            ///------------------------------:
            ConfigRender cfgr; ConfigRender::p = &cfgr; cfgr.init();
            ConfigGame   cfgg; ConfigGame  ::p = &cfgg; cfgg.init();

            ///------------------------------|
            /// Запуск теста...              |
            ///------------------------------:
            std::unique_ptr<RenderLoop> testmain(new RenderLoop);
                                        testmain->run();
        }
};


#endif // PROGRAMM_H
