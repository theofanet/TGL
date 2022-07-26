#pragma once

#include "TGL/Core/Events.h"
#include "TGL/Scene/Entity.h"


class EntitySelectedEvent : Event {
public:
	EntitySelectedEvent(Entity e) : Ent(e) {}
	Entity Ent;
};
