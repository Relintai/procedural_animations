/*
Copyright (c) 2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PROCEDURAL_ANIMATION_PLAYER_H
#define PROCEDURAL_ANIMATION_PLAYER_H

#include "scene/main/node.h"

#include "procedural_animation.h"

class ProceduralAnimationPlayer : public Node {
	GDCLASS(ProceduralAnimationPlayer, Node);

public:
	Ref<ProceduralAnimation> get_animation();
	void set_animation(const Ref<ProceduralAnimation> &animation);

	void play(const StringName &p_name = StringName(), float p_custom_blend = -1, float p_custom_scale = 1.0, bool p_from_end = false);

	ProceduralAnimationPlayer();
	~ProceduralAnimationPlayer();

protected:
	static void _bind_methods();

private:
	Ref<ProceduralAnimation> _animation;
};

#endif