#pragma once
#include <string>
#include <memory>

namespace honse {

    class Fonts {

    public:

        Fonts() = delete;

        static bool Init();
        
        static bool LoadFont(std::string path);

    private:

        struct Impl;
        static std::unique_ptr<Impl> impl;


    };

}