#pragma once
#include "Preferences.h"
#include <bitset>
#include <cstdint>

// The ID of an individual entity slot.
using Entity = unsigned int; 

// Independent component ID.
using Component = unsigned int; 

// Position of the component type in the entity signature.
using ComponentType = uint8_t;

// Describes which components are assigned to the entity index.
using Signature = std::bitset<MAX_COMPONENTS>; 

