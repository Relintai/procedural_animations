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

#include "procedural_animation_player.h"

Ref<ProceduralAnimation> ProceduralAnimationPlayer::get_animation() {
	return _animation;
}
void ProceduralAnimationPlayer::set_animation(const Ref<ProceduralAnimation> &animation) {
	_animation = animation;
}

void ProceduralAnimationPlayer::play(const StringName &p_name, float p_custom_blend, float p_custom_scale, bool p_from_end) {
}

ProceduralAnimationPlayer::ProceduralAnimationPlayer() {
}
ProceduralAnimationPlayer::~ProceduralAnimationPlayer() {
	_animation.unref();
}

void ProceduralAnimationPlayer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_animation"), &ProceduralAnimationPlayer::get_animation);
	ClassDB::bind_method(D_METHOD("set_animation", "value"), &ProceduralAnimationPlayer::set_animation);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation", PROPERTY_HINT_RESOURCE_TYPE, "ProceduralAnimation"), "set_animation", "get_animation");
}