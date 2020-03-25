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

int ProceduralAnimationPlayer::get_current_animation() const {
	return _current_animation;
}
void ProceduralAnimationPlayer::set_current_animation(const int p_animation) {
	_current_animation = p_animation;
}

int ProceduralAnimationPlayer::get_curent_keyframe() const {
	return _curent_keyframe;
}
void ProceduralAnimationPlayer::set_curent_keyframe(const int p_keyframe) {
	_curent_keyframe = p_keyframe;
}

float ProceduralAnimationPlayer::get_scale() const {
	return _scale;
}
void ProceduralAnimationPlayer::set_scale(const float p_scale) {
	_scale = p_scale;
}

bool ProceduralAnimationPlayer::is_playing() const {
	return _playing;
}

void ProceduralAnimationPlayer::play() {
	if (_playing)
		return;

	_playing = true;
}
void ProceduralAnimationPlayer::stop() {
	if (!_playing)
		return;

	_playing = false;
}
void ProceduralAnimationPlayer::setup_frame() {
}
void ProceduralAnimationPlayer::advance(float p_delta) {
}

ProceduralAnimationPlayer::ProceduralAnimationPlayer() {
	_current_animation = 0;
	_curent_keyframe = 0;
	_scale = 1.0;
	_playing = false;
}
ProceduralAnimationPlayer::~ProceduralAnimationPlayer() {
	_animation.unref();
}

void ProceduralAnimationPlayer::_validate_property(PropertyInfo &property) const {

	if (property.name == "current_animation") {

		if (_animation.is_valid()) {

			String names;

			PoolIntArray arr = _animation->get_animation_indices();
			for (int i = 0; i < arr.size(); ++i) {
				if (i > 0)
					names += ",";

				names += _animation->get_animation_name(arr[i]);
			}

			property.hint = PROPERTY_HINT_ENUM;
			property.hint_string = names;
		} else {
			property.hint = PROPERTY_HINT_NONE;
			property.hint_string = "";
		}
	}
}

void ProceduralAnimationPlayer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_animation"), &ProceduralAnimationPlayer::get_animation);
	ClassDB::bind_method(D_METHOD("set_animation", "value"), &ProceduralAnimationPlayer::set_animation);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation", PROPERTY_HINT_RESOURCE_TYPE, "ProceduralAnimation"), "set_animation", "get_animation");

	ClassDB::bind_method(D_METHOD("get_current_animation"), &ProceduralAnimationPlayer::get_current_animation);
	ClassDB::bind_method(D_METHOD("set_current_animation", "animation"), &ProceduralAnimationPlayer::set_current_animation);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_animation"), "set_current_animation", "get_current_animation");

	ClassDB::bind_method(D_METHOD("get_curent_keyframe"), &ProceduralAnimationPlayer::get_curent_keyframe);
	ClassDB::bind_method(D_METHOD("set_curent_keyframe", "animation"), &ProceduralAnimationPlayer::set_curent_keyframe);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "curent_keyframe"), "set_curent_keyframe", "get_curent_keyframe");

	ClassDB::bind_method(D_METHOD("get_scale"), &ProceduralAnimationPlayer::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "animation"), &ProceduralAnimationPlayer::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("is_playing"), &ProceduralAnimationPlayer::is_playing);

	ClassDB::bind_method(D_METHOD("play"), &ProceduralAnimationPlayer::play);
	ClassDB::bind_method(D_METHOD("stop"), &ProceduralAnimationPlayer::stop);
	ClassDB::bind_method(D_METHOD("setup_frame"), &ProceduralAnimationPlayer::setup_frame);
	ClassDB::bind_method(D_METHOD("advance", "delta"), &ProceduralAnimationPlayer::advance);
}