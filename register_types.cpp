#include "register_types.h"

#include "procedural_animation.h"
#include "procedural_animation_player.h"

void register_procedural_animations_types() {
	ClassDB::register_class<ProceduralAnimation>();
	ClassDB::register_class<ProceduralAnimationPlayer>();
}

void unregister_procedural_animations_types() {
}
