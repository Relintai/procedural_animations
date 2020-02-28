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

//Categories
PoolVector<int> ProceduralAnimation::get_category_indices() const {
	PoolVector<int> idxr;
	idxr.resize(_categories.size());

	int i = 0;
	for (Map<int, Category *>::Element *E = _categories.front(); E; E = E->next()) {
		idxr.set(i, E->key());
		++i;
	}

	return idxr;
}
int ProceduralAnimation::add_category(const String &name) {
	Category *cat = memnew(Category);
	cat->name = name;

	int key = -1;
	for (Map<int, Category *>::Element *E = _categories.front(); E; E = E->next()) {
		if (E->key() > key) {
			key = E->key();
		}
	}
	++key;

	_categories[key] = cat;

	return key;
}
void ProceduralAnimation::remove_category(const int index) {
	ERR_FAIL_COND(!_categories.has(index));

	Category *category = _categories[index];

	_categories.erase(index);

	memdelete(category);
}

String ProceduralAnimation::get_category_name(const int category_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), "");

	return _categories[category_index]->name;
}
void ProceduralAnimation::set_category_name(const int category_index, const String &value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	_categories[category_index]->name = value;
}

//Animations
PoolVector<int> ProceduralAnimation::get_animation_indices(const int category_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), PoolVector<int>());

	Category *cat = _categories[category_index];

	PoolVector<int> idxr;
	idxr.resize(cat->animations.size());

	int i = 0;
	for (Map<int, AnimationEntry *>::Element *E = cat->animations.front(); E; E = E->next()) {
		idxr.set(i, E->key());
		++i;
	}

	return idxr;
}
int ProceduralAnimation::add_animation(const int category_index) {
	ERR_FAIL_COND_V(!_categories.has(category_index), 0);

	Category *cat = _categories[category_index];

	int key = -1;
	for (Map<int, AnimationEntry *>::Element *E = cat->animations.front(); E; E = E->next()) {
		if (E->key() > key) {
			key = E->key();
		}
	}
	++key;

	AnimationEntry *entry = memnew(AnimationEntry);

	cat->animations[key] = entry;

	return key;
}
void ProceduralAnimation::remove_animation(const int category_index, const int animation_index) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	AnimationEntry *entry = cat->animations[animation_index];

	cat->animations.erase(animation_index);

	memdelete(entry);
}

String ProceduralAnimation::get_animation_name(const int category_index, const int animation_index) {
	ERR_FAIL_COND_V(!_categories.has(category_index), "");

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), "");

	return cat->animations[animation_index]->name;
}
void ProceduralAnimation::set_animation_name(const int category_index, const int animation_index, const String &value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	cat->animations[animation_index]->name = value;
}

Vector2 ProceduralAnimation::get_animation_node_position(const int category_index, int animation_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), Vector2());

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), Vector2());

	return cat->animations[animation_index]->position;
}
void ProceduralAnimation::set_animation_node_position(const int category_index, const int animation_index, const Vector2 &value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	cat->animations[animation_index]->position = value;
}

int ProceduralAnimation::get_animation_start_frame_index(const int category_index, const int animation_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), 0);

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), 0);

	return cat->animations[animation_index]->start_frame_index;
}
void ProceduralAnimation::set_animation_start_frame_index(const int category_index, const int animation_index, const int value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	cat->animations[animation_index]->start_frame_index = value;
}

//Keyframes
PoolVector<int> ProceduralAnimation::get_keyframe_indices(const int category_index, const int animation_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), PoolVector<int>());

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), PoolVector<int>());

	AnimationEntry *ae = cat->animations[animation_index];

	PoolVector<int> idxr;
	idxr.resize(ae->keyframes.size());

	int i = 0;
	for (Map<int, AnimationKeyFrame *>::Element *E = ae->keyframes.front(); E; E = E->next()) {
		idxr.set(i, E->key());
		++i;
	}

	return idxr;
}
int ProceduralAnimation::add_keyframe(const int category_index, const int animation_index) {
	ERR_FAIL_COND_V(!_categories.has(category_index), 0);

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), 0);

	AnimationEntry *ae = cat->animations[animation_index];

	int key = -1;
	for (Map<int, AnimationKeyFrame *>::Element *E = ae->keyframes.front(); E; E = E->next()) {
		if (E->key() > key) {
			key = E->key();
		}
	}
	++key;

	AnimationKeyFrame *entry = memnew(AnimationKeyFrame);

	ae->keyframes[key] = entry;

	return key;
}
void ProceduralAnimation::remove_keyframe(const int category_index, const int animation_index, const int keyframe_index) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND(!ae->keyframes.has(keyframe_index));

	AnimationKeyFrame *entry = ae->keyframes[keyframe_index];

	cat->animations.erase(keyframe_index);

	memdelete(entry);
}

String ProceduralAnimation::get_keyframe_name(const int category_index, const int animation_index, const int keyframe_index) {
	ERR_FAIL_COND_V(!_categories.has(category_index), "");

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), "");

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND_V(!ae->keyframes.has(keyframe_index), "");

	return ae->keyframes[keyframe_index]->name;
}
void ProceduralAnimation::set_keyframe_name(const int category_index, const int animation_index, const int keyframe_index, const String &value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND(!ae->keyframes.has(keyframe_index));

	ae->keyframes[keyframe_index]->name = value;
}

int ProceduralAnimation::get_keyframe_animation_keyframe_index(const int category_index, int animation_index, const int keyframe_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), 0);

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), 0);

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND_V(!ae->keyframes.has(keyframe_index), 0);

	return ae->keyframes[keyframe_index]->animation_keyframe_index;
}
void ProceduralAnimation::set_keyframe_animation_keyframe_index(const int category_index, int animation_index, const int keyframe_index, int value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND(!ae->keyframes.has(keyframe_index));

	ae->keyframes[keyframe_index]->animation_keyframe_index = value;
}

int ProceduralAnimation::get_keyframe_next_keyframe_index(const int category_index, const int animation_index, const int keyframe_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), 0);

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), 0);

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND_V(!ae->keyframes.has(keyframe_index), 0);

	return ae->keyframes[keyframe_index]->next_keyframe;
}
void ProceduralAnimation::set_keyframe_next_keyframe_index(const int category_index, const int animation_index, const int keyframe_index, const int value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND(!ae->keyframes.has(keyframe_index));

	ae->keyframes[keyframe_index]->next_keyframe = value;
}

Ref<Curve> ProceduralAnimation::get_keyframe_in_curve(const int category_index, const int animation_index, const int keyframe_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), Ref<Curve>());

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), Ref<Curve>());

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND_V(!ae->keyframes.has(keyframe_index), Ref<Curve>());

	return ae->keyframes[keyframe_index]->in_curve;
}
void ProceduralAnimation::set_keyframe_in_curve(const int category_index, const int animation_index, const int keyframe_index, const Ref<Curve> &value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND(!ae->keyframes.has(keyframe_index));

	ae->keyframes[keyframe_index]->in_curve = value;
}

Vector2 ProceduralAnimation::get_keyframe_node_position(const int category_index, int animation_index, const int keyframe_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), Vector2());

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), Vector2());

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND_V(!ae->keyframes.has(keyframe_index), Vector2());

	return ae->keyframes[keyframe_index]->position;
}
void ProceduralAnimation::set_keyframe_node_position(const int category_index, const int animation_index, const int keyframe_index, const Vector2 &value) {
	ERR_FAIL_COND(!_categories.has(category_index));

	Category *cat = _categories[category_index];

	ERR_FAIL_COND(!cat->animations.has(animation_index));

	AnimationEntry *ae = cat->animations[animation_index];

	ERR_FAIL_COND(!ae->keyframes.has(keyframe_index));

	ae->keyframes[keyframe_index]->position = value;
}

void ProceduralAnimation::initialize() {
	ERR_FAIL_COND(!_animation.is_valid());

	for (Map<int, Vector<AnimationKey> *>::Element *E = _animation_data.front(); E; E = E->next()) {
		Vector<AnimationKey> *data = E->get();
		data->clear();
		memdelete(data);
	}

	_animation_data.clear();

	for (Map<int, Category *>::Element *E = _categories.front(); E; E = E->next()) {
		Category *category = E->get();

		for (Map<int, AnimationEntry *>::Element *A = category->animations.front(); A; A = A->next()) {
			AnimationEntry *anim_entry = A->get();

			for (Map<int, AnimationKeyFrame *>::Element *K = anim_entry->keyframes.front(); K; K = K->next()) {
				int keyframe_index = K->get()->animation_keyframe_index;

				if (!_animation_data.has(keyframe_index))
					load_keyframe_data(keyframe_index);
			}
		}
	}
}

void ProceduralAnimation::load_keyframe_data(int keyframe_index) {
	ERR_FAIL_COND(!_animation.is_valid());

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
	}
}

ProceduralAnimation::ProceduralAnimation() {
	_initialized = false;
	_animation_fps = 15;
}

ProceduralAnimation::~ProceduralAnimation() {
	for (Map<int, Category *>::Element *E = _categories.front(); E; E = E->next()) {
		memdelete(E->get());
	}

	for (Map<int, Vector<AnimationKey> *>::Element *E = _animation_data.front(); E; E = E->next()) {
		Vector<AnimationKey> *data = E->get();
		data->clear();
		memdelete(data);
	}

	_animation_data.clear();

	_categories.clear();

	_animation.unref();
}

ProceduralAnimation::TransformAnimationKey ProceduralAnimation::_interpolate(const ProceduralAnimation::TransformAnimationKey &p_a, const ProceduralAnimation::TransformAnimationKey &p_b, float p_c) const {

	TransformAnimationKey ret;
	ret.loc = _interpolate(p_a.loc, p_b.loc, p_c);
	ret.rot = _interpolate(p_a.rot, p_b.rot, p_c);
	ret.scale = _interpolate(p_a.scale, p_b.scale, p_c);

	return ret;
}

Vector3 ProceduralAnimation::_interpolate(const Vector3 &p_a, const Vector3 &p_b, float p_c) const {

	return p_a.linear_interpolate(p_b, p_c);
}
Quat ProceduralAnimation::_interpolate(const Quat &p_a, const Quat &p_b, float p_c) const {

	return p_a.slerp(p_b, p_c);
}
Variant ProceduralAnimation::_interpolate(const Variant &p_a, const Variant &p_b, float p_c) const {

	Variant dst;
	Variant::interpolate(p_a, p_b, p_c, dst);
	return dst;
}

float ProceduralAnimation::_interpolate(const float &p_a, const float &p_b, float p_c) const {

	return p_a * (1.0 - p_c) + p_b * p_c;
}

ProceduralAnimation::TransformAnimationKey ProceduralAnimation::_cubic_interpolate(const ProceduralAnimation::TransformAnimationKey &p_pre_a, const ProceduralAnimation::TransformAnimationKey &p_a, const ProceduralAnimation::TransformAnimationKey &p_b, const ProceduralAnimation::TransformAnimationKey &p_post_b, float p_c) const {

	TransformAnimationKey tk;

	tk.loc = p_a.loc.cubic_interpolate(p_b.loc, p_pre_a.loc, p_post_b.loc, p_c);
	tk.scale = p_a.scale.cubic_interpolate(p_b.scale, p_pre_a.scale, p_post_b.scale, p_c);
	tk.rot = p_a.rot.cubic_slerp(p_b.rot, p_pre_a.rot, p_post_b.rot, p_c);

	return tk;
}
Vector3 ProceduralAnimation::_cubic_interpolate(const Vector3 &p_pre_a, const Vector3 &p_a, const Vector3 &p_b, const Vector3 &p_post_b, float p_c) const {

	return p_a.cubic_interpolate(p_b, p_pre_a, p_post_b, p_c);
}
Quat ProceduralAnimation::_cubic_interpolate(const Quat &p_pre_a, const Quat &p_a, const Quat &p_b, const Quat &p_post_b, float p_c) const {

	return p_a.cubic_slerp(p_b, p_pre_a, p_post_b, p_c);
}
Variant ProceduralAnimation::_cubic_interpolate(const Variant &p_pre_a, const Variant &p_a, const Variant &p_b, const Variant &p_post_b, float p_c) const {

	Variant::Type type_a = p_a.get_type();
	Variant::Type type_b = p_b.get_type();
	Variant::Type type_pa = p_pre_a.get_type();
	Variant::Type type_pb = p_post_b.get_type();

	//make int and real play along

	uint32_t vformat = 1 << type_a;
	vformat |= 1 << type_b;
	vformat |= 1 << type_pa;
	vformat |= 1 << type_pb;

	if (vformat == ((1 << Variant::INT) | (1 << Variant::REAL)) || vformat == (1 << Variant::REAL)) {
		//mix of real and int

		real_t p0 = p_pre_a;
		real_t p1 = p_a;
		real_t p2 = p_b;
		real_t p3 = p_post_b;

		float t = p_c;
		float t2 = t * t;
		float t3 = t2 * t;

		return 0.5f * ((p1 * 2.0f) +
							  (-p0 + p2) * t +
							  (2.0f * p0 - 5.0f * p1 + 4 * p2 - p3) * t2 +
							  (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);

	} else if ((vformat & (vformat - 1))) {

		return p_a; //can't interpolate, mix of types
	}

	switch (type_a) {

		case Variant::VECTOR2: {

			Vector2 a = p_a;
			Vector2 b = p_b;
			Vector2 pa = p_pre_a;
			Vector2 pb = p_post_b;

			return a.cubic_interpolate(b, pa, pb, p_c);
		}
		case Variant::RECT2: {

			Rect2 a = p_a;
			Rect2 b = p_b;
			Rect2 pa = p_pre_a;
			Rect2 pb = p_post_b;

			return Rect2(
					a.position.cubic_interpolate(b.position, pa.position, pb.position, p_c),
					a.size.cubic_interpolate(b.size, pa.size, pb.size, p_c));
		}
		case Variant::VECTOR3: {

			Vector3 a = p_a;
			Vector3 b = p_b;
			Vector3 pa = p_pre_a;
			Vector3 pb = p_post_b;

			return a.cubic_interpolate(b, pa, pb, p_c);
		}
		case Variant::QUAT: {

			Quat a = p_a;
			Quat b = p_b;
			Quat pa = p_pre_a;
			Quat pb = p_post_b;

			return a.cubic_slerp(b, pa, pb, p_c);
		}
		case Variant::AABB: {

			AABB a = p_a;
			AABB b = p_b;
			AABB pa = p_pre_a;
			AABB pb = p_post_b;

			return AABB(
					a.position.cubic_interpolate(b.position, pa.position, pb.position, p_c),
					a.size.cubic_interpolate(b.size, pa.size, pb.size, p_c));
		}
		default: {

			return _interpolate(p_a, p_b, p_c);
		}
	}
}
float ProceduralAnimation::_cubic_interpolate(const float &p_pre_a, const float &p_a, const float &p_b, const float &p_post_b, float p_c) const {

	return _interpolate(p_a, p_b, p_c);
}

template <class T>
T ProceduralAnimation::_interpolate(const Vector<ProceduralAnimation::AnimationKey> &p_keys, float p_time, ProceduralAnimation::KeyInterpolationType p_interp, bool p_loop_wrap, bool *p_ok) const {
	/*
	int len = _find(p_keys, length) + 1; // try to find last key (there may be more past the end)

	if (len <= 0) {
		// (-1 or -2 returned originally) (plus one above)
		// meaning no keys, or only key time is larger than length
		if (p_ok)
			*p_ok = false;
		return T();
	} else if (len == 1) { // one key found (0+1), return it

		if (p_ok)
			*p_ok = true;
		return p_keys[0].value;
	}

	int idx = _find(p_keys, p_time);

	ERR_FAIL_COND_V(idx == -2, T());

	bool result = true;
	int next = 0;
	float c = 0;
	// prepare for all cases of interpolation

	if (loop && p_loop_wrap) {
		// loop
		if (idx >= 0) {

			if ((idx + 1) < len) {

				next = idx + 1;
				float delta = p_keys[next].time - p_keys[idx].time;
				float from = p_time - p_keys[idx].time;

				if (Math::is_zero_approx(delta))
					c = 0;
				else
					c = from / delta;

			} else {

				next = 0;
				float delta = (length - p_keys[idx].time) + p_keys[next].time;
				float from = p_time - p_keys[idx].time;

				if (Math::is_zero_approx(delta))
					c = 0;
				else
					c = from / delta;
			}

		} else {
			// on loop, behind first key
			idx = len - 1;
			next = 0;
			float endtime = (length - p_keys[idx].time);
			if (endtime < 0) // may be keys past the end
				endtime = 0;
			float delta = endtime + p_keys[next].time;
			float from = endtime + p_time;

			if (Math::is_zero_approx(delta))
				c = 0;
			else
				c = from / delta;
		}

	} else { // no loop

		if (idx >= 0) {

			if ((idx + 1) < len) {

				next = idx + 1;
				float delta = p_keys[next].time - p_keys[idx].time;
				float from = p_time - p_keys[idx].time;

				if (Math::is_zero_approx(delta))
					c = 0;
				else
					c = from / delta;

			} else {

				next = idx;
			}

		} else {

			// only allow extending first key to anim start if looping
			if (loop)
				idx = next = 0;
			else
				result = false;
		}
	}

	if (p_ok)
		*p_ok = result;
	if (!result)
		return T();

	float tr = p_keys[idx].transition;

	if (tr == 0 || idx == next) {
		// don't interpolate if not needed
		return p_keys[idx].value;
	}

	if (tr != 1.0) {

		c = Math::ease(c, tr);
	}

	switch (p_interp) {

		case INTERPOLATION_NEAREST: {

			return p_keys[idx].value;
		} break;
		case INTERPOLATION_LINEAR: {

			return _interpolate(p_keys[idx].value, p_keys[next].value, c);
		} break;
		case INTERPOLATION_CUBIC: {
			int pre = idx - 1;
			if (pre < 0)
				pre = 0;
			int post = next + 1;
			if (post >= len)
				post = next;

			return _cubic_interpolate(p_keys[pre].value, p_keys[idx].value, p_keys[next].value, p_keys[post].value, c);

		} break;
		default: return p_keys[idx].value;
	}
*/
	// do a barrel roll

	return T();
}

/*
Error ProceduralAnimation::transform_track_interpolate(int p_track, float p_time, Vector3 *r_loc, Quat *r_rot, Vector3 *r_scale) const {

	ERR_FAIL_INDEX_V(p_track, tracks.size(), ERR_INVALID_PARAMETER);
	Track *t = tracks[p_track];
	ERR_FAIL_COND_V(t->type != TYPE_TRANSFORM, ERR_INVALID_PARAMETER);

	TransformTrack *tt = static_cast<TransformTrack *>(t);

	bool ok = false;

	TransformKey tk = _interpolate(tt->transforms, p_time, tt->interpolation, tt->loop_wrap, &ok);

	if (!ok)
		return ERR_UNAVAILABLE;

	if (r_loc)
		*r_loc = tk.loc;

	if (r_rot)
		*r_rot = tk.rot;

	if (r_scale)
		*r_scale = tk.scale;

	return OK;
}

Variant ProceduralAnimation::value_track_interpolate(int p_track, float p_time) const {

	ERR_FAIL_INDEX_V(p_track, tracks.size(), 0);
	Track *t = tracks[p_track];
	ERR_FAIL_COND_V(t->type != TYPE_VALUE, Variant());
	ValueTrack *vt = static_cast<ValueTrack *>(t);

	bool ok = false;

	Variant res = _interpolate(vt->values, p_time, (vt->update_mode == UPDATE_CONTINUOUS || vt->update_mode == UPDATE_CAPTURE) ? vt->interpolation : INTERPOLATION_NEAREST, vt->loop_wrap, &ok);

	if (ok) {

		return res;
	}

	return Variant();
}
*/

bool ProceduralAnimation::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("categories/")) {

		int category_index = name.get_slicec('/', 1).to_int();
		String what = name.get_slicec('/', 2);

		if (!_categories.has(category_index)) {
			Category *cat = memnew(Category);

			_categories[category_index] = cat;
		}

		Category *cat = _categories[category_index];

		if (what == "name") {
			cat->name = p_value;

			return true;
		} else if (what == "animation") {
			int animation_index = name.get_slicec('/', 3).to_int();
			String anim_prop_name = name.get_slicec('/', 4);

			if (!cat->animations.has(animation_index)) {
				AnimationEntry *ae = memnew(AnimationEntry);

				cat->animations[animation_index] = ae;
			}

			AnimationEntry *ae = cat->animations[animation_index];

			if (anim_prop_name == "name") {
				ae->name = p_value;

				return true;
			} else if (anim_prop_name == "position") {
				ae->position = p_value;

				return true;
			} else if (anim_prop_name == "start_frame_index") {
				ae->start_frame_index = p_value;

				return true;
			} else if (anim_prop_name == "keyframe") {
				int keyframe_index = name.get_slicec('/', 5).to_int();
				String keyframe_name = name.get_slicec('/', 6);

				if (!ae->keyframes.has(keyframe_index)) {
					AnimationKeyFrame *keyframe = memnew(AnimationKeyFrame);

					ae->keyframes[keyframe_index] = keyframe;
				}

				AnimationKeyFrame *keyframe = ae->keyframes[keyframe_index];

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
		}
	} else {
		return false;
	}

	return true;
}

bool ProceduralAnimation::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("categories/")) {

		int category_index = name.get_slicec('/', 1).to_int();
		String what = name.get_slicec('/', 2);

		Category *category = _categories[category_index];

		if (what == "name") {
			r_ret = category->name;

			return true;
		} else if (what == "animation") {
			int animation_index = name.get_slicec('/', 3).to_int();
			String anim_prop_name = name.get_slicec('/', 4);

			AnimationEntry *anim = category->animations[animation_index];

			if (anim_prop_name == "name") {
				r_ret = anim->name;

				return true;
			} else if (anim_prop_name == "position") {
				r_ret = anim->position;

				return true;
			} else if (anim_prop_name == "start_frame_index") {
				r_ret = anim->start_frame_index;

				return true;
			} else if (anim_prop_name == "keyframe") {
				int keyframe_index = name.get_slicec('/', 5).to_int();
				String keyframe_prop_name = name.get_slicec('/', 6);

				AnimationKeyFrame *keyframe = anim->keyframes[keyframe_index];

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

	for (Map<int, Category *>::Element *E = _categories.front(); E; E = E->next()) {
		Category *category = E->get();

		p_list->push_back(PropertyInfo(Variant::STRING, "categories/" + itos(E->key()) + "/name", PROPERTY_HINT_NONE, "", property_usange));

		for (Map<int, AnimationEntry *>::Element *A = category->animations.front(); A; A = A->next()) {
			AnimationEntry *animation = A->get();

			p_list->push_back(PropertyInfo(Variant::STRING, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/name", PROPERTY_HINT_NONE, "", property_usange));
			p_list->push_back(PropertyInfo(Variant::VECTOR2, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/position", PROPERTY_HINT_NONE, "", property_usange));
			p_list->push_back(PropertyInfo(Variant::INT, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/start_frame_index", PROPERTY_HINT_NONE, "", property_usange));

			for (Map<int, AnimationKeyFrame *>::Element *K = animation->keyframes.front(); K; K = K->next()) {
				p_list->push_back(PropertyInfo(Variant::STRING, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/name", PROPERTY_HINT_NONE, "", property_usange));
				p_list->push_back(PropertyInfo(Variant::INT, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/animation_keyframe_index", PROPERTY_HINT_NONE, "", property_usange));
				p_list->push_back(PropertyInfo(Variant::INT, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/next_keyframe", PROPERTY_HINT_NONE, "", property_usange));
				p_list->push_back(PropertyInfo(Variant::OBJECT, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/in_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve", property_usange));
				p_list->push_back(PropertyInfo(Variant::VECTOR2, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/position", PROPERTY_HINT_NONE, "", property_usange));
			}
		}
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

	//Categories
	ClassDB::bind_method(D_METHOD("get_category_indices"), &ProceduralAnimation::get_category_indices);
	ClassDB::bind_method(D_METHOD("add_category", "name"), &ProceduralAnimation::add_category);
	ClassDB::bind_method(D_METHOD("remove_category", "index"), &ProceduralAnimation::remove_category);

	ClassDB::bind_method(D_METHOD("get_category_name", "category_index"), &ProceduralAnimation::get_category_name);
	ClassDB::bind_method(D_METHOD("set_category_name", "category_index"), &ProceduralAnimation::set_category_name);

	//Animations
	ClassDB::bind_method(D_METHOD("get_animation_indices", "category_index"), &ProceduralAnimation::get_animation_indices);
	ClassDB::bind_method(D_METHOD("add_animation", "category_index"), &ProceduralAnimation::add_animation);
	ClassDB::bind_method(D_METHOD("remove_animation", "category_index", "animation_index"), &ProceduralAnimation::remove_animation);

	ClassDB::bind_method(D_METHOD("get_animation_name", "category_index", "animation_index"), &ProceduralAnimation::get_animation_name);
	ClassDB::bind_method(D_METHOD("set_animation_name", "category_index", "animation_index", "value"), &ProceduralAnimation::set_animation_name);

	ClassDB::bind_method(D_METHOD("get_animation_node_position", "category_index", "animation_index"), &ProceduralAnimation::get_animation_node_position);
	ClassDB::bind_method(D_METHOD("set_animation_node_position", "category_index", "animation_index", "value"), &ProceduralAnimation::set_animation_node_position);

	ClassDB::bind_method(D_METHOD("get_animation_start_frame_index", "category_index", "animation_index"), &ProceduralAnimation::get_animation_start_frame_index);
	ClassDB::bind_method(D_METHOD("set_animation_start_frame_index", "category_index", "animation_index", "value"), &ProceduralAnimation::set_animation_start_frame_index);

	//Keyframes
	ClassDB::bind_method(D_METHOD("get_keyframe_indices", "category_index", "animation_index"), &ProceduralAnimation::get_keyframe_indices);
	ClassDB::bind_method(D_METHOD("add_keyframe", "category_index", "animation_index"), &ProceduralAnimation::add_keyframe);
	ClassDB::bind_method(D_METHOD("remove_keyframe", "category_index", "animation_index", "keyframe_index"), &ProceduralAnimation::remove_keyframe);

	ClassDB::bind_method(D_METHOD("get_keyframe_name", "category_index", "animation_index", "keyframe_index"), &ProceduralAnimation::get_keyframe_name);
	ClassDB::bind_method(D_METHOD("set_keyframe_name", "category_index", "animation_index", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_name);

	ClassDB::bind_method(D_METHOD("get_keyframe_animation_keyframe_index", "category_index", "animation_index", "keyframe_index"), &ProceduralAnimation::get_keyframe_animation_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_keyframe_animation_keyframe_index", "category_index", "animation_index", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_animation_keyframe_index);

	ClassDB::bind_method(D_METHOD("get_keyframe_next_keyframe_index", "category_index", "animation_index", "keyframe_index"), &ProceduralAnimation::get_keyframe_next_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_keyframe_next_keyframe_index", "category_index", "animation_index", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_next_keyframe_index);

	ClassDB::bind_method(D_METHOD("get_keyframe_in_curve", "category_index", "animation_index", "keyframe_index"), &ProceduralAnimation::get_keyframe_in_curve);
	ClassDB::bind_method(D_METHOD("set_keyframe_in_curve", "category_index", "animation_index", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_in_curve);

	ClassDB::bind_method(D_METHOD("get_keyframe_node_position", "category_index", "animation_index", "keyframe_index"), &ProceduralAnimation::get_keyframe_node_position);
	ClassDB::bind_method(D_METHOD("set_keyframe_node_position", "category_index", "animation_index", "keyframe_index", "value"), &ProceduralAnimation::set_keyframe_node_position);

	ClassDB::bind_method(D_METHOD("initialize"), &ProceduralAnimation::initialize);
	ClassDB::bind_method(D_METHOD("load_keyframe_data", "keyframe_index"), &ProceduralAnimation::load_keyframe_data);
}