#pragma once
#include <glm/glm.hpp>
 
namespace hs {

    typedef unsigned int ObjectID;

    class Object {

    public:

        Object();
        virtual ~Object();

        bool active;

        ObjectID ID;

    };

}