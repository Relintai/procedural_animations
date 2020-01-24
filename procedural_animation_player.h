#ifndef PROCEDURAL_ANIMATION_PLAYER_H
#define PROCEDURAL_ANIMATION_PLAYER_H

#include "scene/main/node.h"

class ProceduralAnimationPlayer : public Node {
	GDCLASS(ProceduralAnimationPlayer, Node);

public:
	void play(const StringName &p_name = StringName(), float p_custom_blend = -1, float p_custom_scale = 1.0, bool p_from_end = false);
};

#endif