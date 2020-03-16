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

int ProceduralAnimationPlayer::get_current_category() const {
	return _current_category;
}
void ProceduralAnimationPlayer::set_current_category(const int p_category) {
	_current_category = p_category;
}

int ProceduralAnimationPlayer::get_current_animation() const {
	return _current_animation;
}
void ProceduralAnimationPlayer::set_current_animation(const int p_animation) {
	_current_animation = p_animation;
}

void ProceduralAnimationPlayer::play(const StringName &p_name, float p_custom_blend, float p_custom_scale, bool p_from_end) {
}

ProceduralAnimationPlayer::ProceduralAnimationPlayer() {
	_current_category = 0;
	_current_animation = 0;
}
ProceduralAnimationPlayer::~ProceduralAnimationPlayer() {
	_animation.unref();
}

void ProceduralAnimationPlayer::_validate_property(PropertyInfo &property) const {

	if (property.name == "current_category") {

		if (_animation.is_valid()) {

			String names;

			PoolIntArray arr = _animation->get_category_indices();
			for (int i = 0; i < arr.size(); ++i) {
				if (i > 0)
					names += ",";

				names += _animation->get_category_name(arr[i]);
			}

			property.hint = PROPERTY_HINT_ENUM;
			property.hint_string = names;
		} else {

			property.hint = PROPERTY_HINT_NONE;
			property.hint_string = "";
		}
	} else if (property.name == "current_animation") {

		if (_animation.is_valid() && _animation->has_category(_current_category)) {

			String names;

			PoolIntArray arr = _animation->get_animation_indices(_current_category);
			for (int i = 0; i < arr.size(); ++i) {
				if (i > 0)
					names += ",";

				names += _animation->get_animation_name(_current_category, arr[i]);
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

	ClassDB::bind_method(D_METHOD("get_current_category"), &ProceduralAnimationPlayer::get_current_category);
	ClassDB::bind_method(D_METHOD("set_current_category", "category"), &ProceduralAnimationPlayer::set_current_category);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_category"), "set_current_category", "get_current_category");

	ClassDB::bind_method(D_METHOD("get_current_animation"), &ProceduralAnimationPlayer::get_current_animation);
	ClassDB::bind_method(D_METHOD("set_current_animation", "animation"), &ProceduralAnimationPlayer::set_current_animation);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_animation"), "set_current_animation", "get_current_animation");
}