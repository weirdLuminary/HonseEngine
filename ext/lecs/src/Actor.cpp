#include "Actor.h"

Actor::Actor(World* world) : m_World(world), m_ID(m_World->CreateEntity()), m_Initialized(true) {};