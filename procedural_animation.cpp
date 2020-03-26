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

#include "procedural_animation.h"

Ref<Animation> ProceduralAnimation::get_animation() const {
	return _animation;
}
void ProceduralAnimation::set_animation(const Ref<Animation> &value) {
	_animation = value;
}

int ProceduralAnimation::get_animation_fps() const {
	return _animation_fps;
}
void ProceduralAnimation::set_animation_fps(const int index) {
	_animation_fps = index;
}

String ProceduralAnimation::get_animation_keyframe_name(int keyframe_index) const {
	if (!_keyframe_names.has(keyframe_index)) {
		return String::num(keyframe_index);
	}

	return String::num(keyframe_index) + " " + _keyframe_names[keyframe_index];
}
void ProceduralAnimation::set_animation_keyframe_name(int keyframe_index, const String &value) {
	_keyframe_names[keyframe_index] = value;
}
void ProceduralAnimation::remove_animation_keyframe_name(int keyframe_index) {
	if (!_keyframe_names.has(keyframe_index)) {
		return;
	}

	_keyframe_names.erase(keyframe_index);
}
PoolVector<String> ProceduralAnimation::get_animation_keyframe_names() const {
	PoolVector<String> names;

	names.resize(_keyframe_names.size());

	int i = 0;
	for (Map<int, String>::Element *E = _keyframe_names.front(); E; E = E->next()) {
		names.set(i, String::num(E->key()) + " " + _keyframe_names[E->key()]);
		++i;
	}

	return names;
}

Vector2 ProceduralAnimation::get_start_node_position() const {
	return _start_node_position;
}
void ProceduralAnimation::set_start_node_position(const Vector2 &value) {
	_start_node_position = value;
}

int ProceduralAnimation::get_start_frame_index() const {
	return _start_frame_index;
}
void ProceduralAnimation::set_start_frame_index(const int value) {
	_start_frame_index = value;
}

//Keyframes
PoolVector<int> ProceduralAnimation::get_keyframe_indices() const {
	PoolVector<int> idxr;
	idxr.resize(_keyframes.size());

	int i = 0;
	for (Map<int, AnimationKeyFrame *>::Element *E = _keyframes.front(); E; E = E->next()) {
		idxr.set(i, E->key());
		++i;
	}

	return idxr;
}
int ProceduralAnimation::add_keyframe() {
	int key = -1;
	for (Map<int, AnimationKeyFrame *>::Element *E = _keyframes.front(); E; E = E->next()) {
		if (E->key() > key) {
			key = E->key();
		}
	}
	++key;

	AnimationKeyFrame *entry = memnew(AnimationKeyFrame);

	_keyframes[key] = entry;

	return key;
}
void ProceduralAnimation::remove_keyframe(const int keyframe_index) {
	ERR_FAIL_COND(!_keyframes.has(keyframe_index));

	AnimationKeyFrame *entry = _keyframes[keyframe_index];

	_keyframes.erase(keyframe_index);

	memdelete(entry);
}

bool ProceduralAnimation::has_keyframe(const int keyframe_index) const {
	return _keyframes.has(keyframe_index);
}

String ProceduralAnimation::get_keyframe_name(const int keyframe_index) const {
	ERR_FAIL_COND_V(!_keyframes.has(keyframe_index), "");

	return _keyframes[keyframe_index]->name;
}
void ProceduralAnimation::set_keyframe_name(const int keyframe_index, const String &value) {
	ERR_FAIL_COND(!_keyframes.has(keyframe_index));

	_keyframes[keyframe_index]->name = value;
}

int ProceduralAnimation::get_keyframe_animation_keyframe_index(const int keyframe_index) const {
	ERR_FAIL_COND_V(!_keyframes.has(keyframe_index), 0);

	return _keyframes[keyframe_index]->animation_keyframe_index;
}
void ProceduralAnimation::set_keyframe_animation_keyframe_index(const int keyframe_index, int value) {
	ERR_FAIL_COND(!_keyframes.has(keyframe_index));

	_keyframes[keyframe_index]->animation_keyframe_index = value;
}

int ProceduralAnimation::get_keyframe_next_keyframe_index(const int keyframe_index) const {
	ERR_FAIL_COND_V(!_keyframes.has(keyframe_index), 0);

	return _keyframes[keyframe_index]->next_keyframe;
}
void ProceduralAnimation::set_keyframe_next_keyframe_index(const int keyframe_index, const int value) {
	ERR_FAIL_COND(!_keyframes.has(keyframe_index));

	_keyframes[keyframe_index]->next_keyframe = value;
}

Ref<Curve> ProceduralAnimation::get_keyframe_in_curve(const int keyframe_index) const {
	ERR_FAIL_COND_V(!_keyframes.has(keyframe_index), Ref<Curve>());

	return _keyframes[keyframe_index]->in_curve;
}
void ProceduralAnimation::set_keyframe_in_curve(const int keyframe_index, const Ref<Curve> &value) {
	ERR_FAIL_COND(!_keyframes.has(keyframe_index));

	_keyframes[keyframe_index]->in_curve = value;
}

Vector2 ProceduralAnimation::get_keyframe_node_position(const int keyframe_index) const {
	ERR_FAIL_COND_V(!_keyframes.has(keyframe_index), Vector2());

	return _keyframes[keyframe_index]->position;
}
void ProceduralAnimation::set_keyframe_node_position(const int keyframe_index, const Vector2 &value) {
	ERR_FAIL_COND(!_keyframes.has(keyframe_index));

	_keyframes[keyframe_index]->position = value;
}

void ProceduralAnimation::initialize() {
	ERR_FAIL_COND(!_animation.is_valid());
	/*
	for (Map<int, Vector<AnimationKey> *>::Element *E = _animation_data.front(); E; E = E->next()) {
		Vector<AnimationKey> *data = E->get();
		data->clear();
		memdelete(data);
	}

	_animation_data.clear();

	for (Map<int, AnimationKeyFrame *>::Element *K = _keyframes.front(); K; K = K->next()) {
		int keyframe_index = K->get()->animation_keyframe_index;

		if (!_animation_data.has(keyframe_index))
			load_keyframe_data(keyframe_index);
	}*/
}

void ProceduralAnimation::load_keyframe_data(int keyframe_index) {
	ERR_FAIL_COND(!_animation.is_valid());
	/*
	Vector<AnimationKey> *vec = NULL;

	if (_animation_data.has(keyframe_index)) {
		//reload data
		vec = _animation_data[keyframe_index];
		vec->clear();
	} else {
		vec = memnew(Vector<AnimationKey>);
	}

	float time = keyframe_index * _animation->get_length() / static_cast<float>(_animation_fps);

	for (int i = 0; i < _animation->get_track_count(); ++i) {
		int key_index = _animation->track_find_key(i, time, true);

		if (key_index == -1)
			continue;

		Animation::TrackType tt = _animation->track_get_type(i);

		switch (tt) {
			case Animation::TYPE_VALUE: {
				VariantAnimationKey d;
				d.path = _animation->track_get_path(key_index);
				d.value = _animation->track_get_key_value(i, key_index);

				vec->push_back(d);
			} break;
			case Animation::TYPE_TRANSFORM: {
				Vector3 loc;
				Quat rot;
				Vector3 scale;

				if (_animation->transform_track_get_key(i, key_index, &loc, &rot, &scale) == OK) {
					TransformAnimationKey d;

					d.path = _animation->track_get_path(key_index);
					d.loc = loc;
					d.rot = rot;
					d.scale = scale;

					vec->push_back(d);
				}
			} break;
			case Animation::TYPE_METHOD: {
				MethodAnimationKey d;
				d.path = _animation->track_get_path(key_index);

				d.method = _animation->method_track_get_name(i, key_index);
				d.params = _animation->method_track_get_params(i, key_index);

				vec->push_back(d);
			} break;
			case Animation::TYPE_BEZIER: {
				//ignore
			} break;
			case Animation::TYPE_AUDIO: {
				AudioAnimationKey d;
				d.path = _animation->track_get_path(key_index);
				d.stream = _animation->audio_track_get_key_stream(i, key_index);
				d.start_offset = _animation->audio_track_get_key_start_offset(i, key_index);
				d.end_offset = _animation->audio_track_get_key_end_offset(i, key_index);

				vec->push_back(d);
			} break;
			case Animation::TYPE_ANIMATION: {
				///ignore
			} break;
		}
	}*/
}

ProceduralAnimation::ProceduralAnimation() {
	_initialized = false;
	_animation_fps = 15;
	_start_frame_index = -1;
}

ProceduralAnimation::~ProceduralAnimation() {
	for (Map<int, AnimationKeyFrame *>::Element *E = _keyframes.front(); E; E = E->next())
		memdelete(E->get());

	_keyframes.clear();

	_animation.unref();
}

bool ProceduralAnimation::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name == "start_node_position") {
		_start_node_position = p_value;

		return true;
	} else if (name == "start_frame_index") {
		_start_frame_index = p_value;

		return true;
	} else if (name == "keyframe") {
		int keyframe_index = name.get_slicec('/', 1).to_int();
		String keyframe_name = name.get_slicec('/', 2);

		if (!_keyframes.has(keyframe_index)) {
			AnimationKeyFrame *keyframe = memnew(AnimationKeyFrame);

			_keyframes[keyframe_index] = keyframe;
		}

		AnimationKeyFrame *keyframe = _keyframes[keyframe_index];

		if (keyframe_name == "name") {
			keyframe->name = p_value;

			return true;
		} else if (keyframe_name == "animation_keyframe_index") {
			keyframe->animation_keyframe_index = p_value;

			return true;
		} else if (keyframe_name == "next_keyframe") {
			keyframe->next_keyframe = p_value;

			return true;
		} else if (keyframe_name == "in_curve") {
			keyframe->in_curve = p_value;

			return true;
		} else if (keyframe_name == "position") {
			keyframe->position = p_value;

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

	return true;
}

bool ProceduralAnimation::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name == "start_node_position") {
		r_ret = _start_node_position;

		return true;
	} else if (name == "start_frame_index") {
		r_ret = _start_frame_index;

		return true;
	} else if (name == "keyframe") {
		int keyframe_index = name.get_slicec('/', 1).to_int();
		String keyframe_prop_name = name.get_slicec('/', 2);

		AnimationKeyFrame *keyframe = _keyframes[keyframe_index];

		if (keyframe_prop_name == "name") {
			r_ret = keyframe->name;

			return true;
		} else if (keyframe_prop_name == "animation_keyframe_index") {
			r_ret = keyframe->animation_keyframe_index;

			return true;
		} else if (keyframe_prop_name == "next_keyframe") {
			r_ret = keyframe->next_keyframe;

			return true;
		} else if (keyframe_prop_name == "in_curve") {
			r_ret = keyframe->in_curve;

			return true;
		} else if (keyframe_prop_name == "position") {
			r_ret = keyframe->position;

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

	return true;
}

void ProceduralAnimation::_get_property_list(List<PropertyInfo> *p_list) const {
	//int property_usange = PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL;
	int property_usange = PROPERTY_USAGE_DEFAULT;

	p_list->push_back(PropertyInfo(Variant::VECTOR2, "position", PROPERTY_HINT_NONE, "", property_usange));
	p_list->push_back(PropertyInfo(Variant::INT, "start_frame_index", PROPERTY_HINT_NONE, "", property_usange));

	for (Map<int, AnimationKeyFrame *>::Element *K = _keyframes.front(); K; K = K->next()) {
		p_list->push_back(PropertyInfo(Variant::STRING, "keyframe/" + itos(K->key()) + "/name", PROPERTY_HINT_NONE, "", property_usange));
		p_list->push_back(PropertyInfo(Variant::INT, "keyframe/" + itos(K->key()) + "/animation_keyframe_index", PROPERTY_HINT_NONE, "", property_usange));
		p_list->push_back(PropertyInfo(Variant::INT, "keyframe/" + itos(K->key()) + "/next_keyframe", PROPERTY_HINT_NONE, "", property_usange));
		p_list->push_back(PropertyInfo(Variant::OBJECT, "keyframe/" + itos(K->key()) + "/in_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve", property_usange));
		p_list->push_back(PropertyInfo(Variant::VECTOR2, "keyframe/" + itos(K->key()) + "/position", PROPERTY_HINT_NONE, "", property_usange));
	}
}

void ProceduralAnimation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_animation"), &ProceduralAnimation::get_animation);
	ClassDB::bind_method(D_METHOD("set_animation", "value"), &ProceduralAnimation::set_animation);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation", PROPERTY_HINT_RESOURCE_TYPE, "Animation"), "set_animation", "get_animation");

	ClassDB::bind_method(D_METHOD("get_animation_fps"), &ProceduralAnimation::get_animation_fps);
	ClassDB::bind_method(D_METHOD("set_animation_fps", "value"), &ProceduralAnimation::set_animation_fps);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "animation_fps"), "set_animation_fps", "get_animation_fps");

	ClassDB::bind_method(D_METHOD("get_animation_keyframe_name", "keyframe_index"), &ProceduralAnimation::get_animation_keyframe_name);
	ClassDB::bind_method(D_METHOD("set_animation_keyframe_name", "keyframe_index", "value"), &ProceduralAnimation::set_animation_keyframe_name);
	ClassDB::bind_method(D_METHOD("remove_animation_keyframe_name", "keyframe_index"), &ProceduralAnimation::remove_animation_keyframe_name);
	ClassDB::bind_method(D_METHOD("get_animation_keyframe_names"), &ProceduralAnimation::get_animation_keyframe_names);

	ClassDB::bind_method(D_METHOD("get_start_node_position"), &ProceduralAnimation::get_start_node_position);
	ClassDB::bind_method(D_METHOD("set_start_node_position", "value"), &ProceduralAnimation::set_start_node_position);

	ClassDB::bind_method(D_METHOD("get_start_frame_index"), &ProceduralAnimation::get_start_frame_index);
	ClassDB::bind_method(D_METHOD("set_start_frame_index", "value"), &ProceduralAnimation::set_start_frame_index);

	ClassDB::bind_method(D_METHOD("get_length"), &ProceduralAnimation::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "time_sec"), &ProceduralAnimation::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length", PROPERTY_HINT_RANGE, "0.001,99999,0.001"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("has_loop"), &ProceduralAnimation::has_loop);
	ClassDB::bind_method(D_METHOD("set_loop", "enabled"), &ProceduralAnimation::set_loop);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "has_loop");

	//Keyframes
	ClassDB::bind_method(D_METHOD("get_keyframe_indices"), &ProceduralAnimation::get_keyframe_indices);
	ClassDB::bind_method(D_METHOD("add_keyframe"), &ProceduralAnimation::add_keyframe);
	ClassDB::bind_method(D_METHOD("remove_keyframe", "keyframe_index"), &ProceduralAnimation::remove_keyframe);
	ClassDB::bind_method(D_METHOD("has_keyframe", "keyframe_index"), &ProceduralAnimation::has_keyframe);

	ClassDB::bind_method(D_METHOD("get_keyframe_name", "keyframe_index"), &ProceduralAnimation::get_keyframe_name);
	ClassDB::bind_method(D_METHOD("set_keyframe_name", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_name);

	ClassDB::bind_method(D_METHOD("get_keyframe_animation_keyframe_index", "keyframe_index"), &ProceduralAnimation::get_keyframe_animation_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_keyframe_animation_keyframe_index", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_animation_keyframe_index);

	ClassDB::bind_method(D_METHOD("get_keyframe_next_keyframe_index", "keyframe_index"), &ProceduralAnimation::get_keyframe_next_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_keyframe_next_keyframe_index", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_next_keyframe_index);

	ClassDB::bind_method(D_METHOD("get_keyframe_in_curve", "keyframe_index"), &ProceduralAnimation::get_keyframe_in_curve);
	ClassDB::bind_method(D_METHOD("set_keyframe_in_curve", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_in_curve);

	ClassDB::bind_method(D_METHOD("get_keyframe_node_position", "keyframe_index"), &ProceduralAnimation::get_keyframe_node_position);
	ClassDB::bind_method(D_METHOD("set_keyframe_node_position", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_node_position);

	ClassDB::bind_method(D_METHOD("initialize"), &ProceduralAnimation::initialize);
	ClassDB::bind_method(D_METHOD("load_keyframe_data", "keyframe_index"), &ProceduralAnimation::load_keyframe_data);
}