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

#ifndef PROCEDURAL_ANIMATION_H
#define PROCEDURAL_ANIMATION_H

#include "scene/resources/animation.h"

#include "core/map.h"
#include "core/math/vector2.h"
#include "core/pool_vector.h"
#include "core/vector.h"

#include "scene/resources/animation.h"
#include "scene/resources/curve.h"

class ProceduralAnimation : public Animation {
	GDCLASS(ProceduralAnimation, Animation);

protected:
	struct AnimationKeyFrame {
		String name;
		int animation_keyframe_index;
		int next_keyframe;
		//Ref<Curve> in_curve;
		Vector2 position;
		float transition;

		AnimationKeyFrame() {
			animation_keyframe_index = 0;
			transition = 1.0;
			next_keyframe = -1;
			//in_curve.instance();
		}

		~AnimationKeyFrame() {
			//in_curve.unref();
		}
	};

public:
	Ref<Animation> get_animation() const;
	void set_animation(const Ref<Animation> &value);

	int get_animation_fps() const;
	void set_animation_fps(const int index);

	String get_animation_keyframe_name(int keyframe_index) const;
	void set_animation_keyframe_name(int keyframe_index, const String &value);
	void remove_animation_keyframe_name(int keyframe_index);
	PoolVector<String> get_animation_keyframe_names() const;

	Vector2 get_start_node_position() const;
	void set_start_node_position(const Vector2 &value);

	int get_start_frame_index() const;
	void set_start_frame_index(const int value);

	//Keyframes
	PoolVector<int> get_keyframe_indices() const;
	int add_keyframe();
	void remove_keyframe(const int keyframe_index);
	bool has_keyframe(const int keyframe_index) const;

	String get_keyframe_name(const int keyframe_index) const;
	void set_keyframe_name(const int keyframe_index, const String &value);

	int get_keyframe_animation_keyframe_index(const int keyframe_index) const;
	void set_keyframe_animation_keyframe_index(const int keyframe_index, const int value);

	int get_keyframe_next_keyframe_index(const int keyframe_index) const;
	void set_keyframe_next_keyframe_index(const int keyframe_index, const int value);

	float get_keyframe_transition(const int keyframe_index) const;
	void set_keyframe_transition(const int keyframe_index, const float value);

	//Ref<Curve> get_keyframe_in_curve(const int keyframe_index) const;
	//void set_keyframe_in_curve(const int keyframe_index, const Ref<Curve> &value);

	Vector2 get_keyframe_node_position(const int keyframe_index) const;
	void set_keyframe_node_position(const int keyframe_index, const Vector2 &value);

	void process_animation_data();
	void load_keyframe_data(const float keyframe_time, const float previous_keyframe_time, const int keyframe_index, const bool interpolation_allowed = false);

	ProceduralAnimation();
	~ProceduralAnimation();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

private:
	bool _initialized;
	int _animation_fps;

	String _editor_add_category_name;
	String _add_editor_category_animation_name;

	Vector2 _start_node_position;
	int _start_frame_index;
	Map<int, AnimationKeyFrame *> _keyframes;

	Ref<Animation> _animation;
	Map<int, String> _keyframe_names;
};

#endif