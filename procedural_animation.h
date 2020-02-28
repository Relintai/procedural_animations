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

#include "core/resource.h"

#include "core/map.h"
#include "core/math/vector2.h"
#include "core/pool_vector.h"
#include "core/vector.h"
#include "scene/resources/animation.h"
#include "scene/resources/curve.h"

#include "scene/resources/animation.h"

class ProceduralAnimation : public Resource {
	GDCLASS(ProceduralAnimation, Resource);

	friend class Animation;

protected:
	enum AnimationKeyTrackType {
		TYPE_VALUE = Animation::TYPE_VALUE,
		TYPE_TRANSFORM = Animation::TYPE_TRANSFORM,
		TYPE_METHOD = Animation::TYPE_METHOD,
		TYPE_BEZIER = Animation::TYPE_BEZIER,
		TYPE_AUDIO = Animation::TYPE_AUDIO,
		TYPE_ANIMATION = Animation::TYPE_ANIMATION,
		TYPE_NONE,
	};

	enum KeyInterpolationType {
		INTERPOLATION_NEAREST = Animation::INTERPOLATION_NEAREST,
		INTERPOLATION_LINEAR = Animation::INTERPOLATION_LINEAR,
		INTERPOLATION_CUBIC = Animation::INTERPOLATION_CUBIC,
		INTERPOLATION_CURVE,
	};

	/* Key data */
	struct AnimationKey {
		AnimationKeyTrackType type;
		NodePath path;
		bool enabled;

		AnimationKey() {
			type = TYPE_NONE;
			enabled = true;
		}
	};

	struct VariantAnimationKey : public AnimationKey {
		Variant value;

		VariantAnimationKey() {
			type = TYPE_VALUE;
		}
	};

	struct TransformAnimationKey : public AnimationKey {
		Vector3 loc;
		Quat rot;
		Vector3 scale;

		TransformAnimationKey() :
				AnimationKey() {
			type = TYPE_TRANSFORM;
		}
	};

	struct MethodAnimationKey : public AnimationKey {
		StringName method;
		Vector<Variant> params;

		MethodAnimationKey() :
				AnimationKey() {
			type = TYPE_METHOD;
		}
	};

	struct AudioAnimationKey : public AnimationKey {
		RES stream;
		float start_offset;
		float end_offset;

		AudioAnimationKey() :
				AnimationKey() {
			type = TYPE_AUDIO;
			start_offset = 0;
			end_offset = 0;
		}
	};

	/* Animation data */

	struct AnimationKeyFrame {
		String name;
		int animation_keyframe_index;
		int next_keyframe;
		Ref<Curve> in_curve;
		Vector2 position;

		AnimationKeyFrame() {
			animation_keyframe_index = 0;
			next_keyframe = -1;
			in_curve.instance();
		}

		~AnimationKeyFrame() {
			in_curve.unref();
		}
	};

	struct AnimationEntry {
		String name;
		Vector2 position;
		int start_frame_index;
		Map<int, AnimationKeyFrame *> keyframes;

		AnimationEntry() {
			start_frame_index = -1;
		}

		~AnimationEntry() {
			for (Map<int, AnimationKeyFrame *>::Element *E = keyframes.front(); E; E = E->next())
				memdelete(E->get());

			keyframes.clear();
		}
	};

	struct Category {
		String name;
		Map<int, AnimationEntry *> animations;

		~Category() {
			for (Map<int, AnimationEntry *>::Element *E = animations.front(); E; E = E->next())
				memdelete(E->get());

			animations.clear();
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

	//Categories
	PoolVector<int> get_category_indices() const;
	int add_category(const String &name);
	void remove_category(const int index);

	String get_category_name(const int category_index) const;
	void set_category_name(const int category_index, const String &value);

	//Animations
	PoolVector<int> get_animation_indices(const int category_index) const;
	int add_animation(const int category_index);
	void remove_animation(const int category_index, const int animation_index);

	String get_animation_name(const int category_index, const int animation_index);
	void set_animation_name(const int category_index, const int animation_index, const String &value);

	Vector2 get_animation_node_position(const int category_index, int animation_index) const;
	void set_animation_node_position(const int category_index, const int animation_index, const Vector2 &value);

	int get_animation_start_frame_index(const int category_index, const int animation_index) const;
	void set_animation_start_frame_index(const int category_index, const int animation_index, const int value);

	//Keyframes
	PoolVector<int> get_keyframe_indices(const int category_index, const int animation_index) const;
	int add_keyframe(const int category_index, const int animation_index);
	void remove_keyframe(const int category_index, const int animation_index, const int keyframe_index);

	String get_keyframe_name(const int category_index, const int animation_index, const int keyframe_index);
	void set_keyframe_name(const int category_index, const int animation_index, const int keyframe_index, const String &value);

	int get_keyframe_animation_keyframe_index(const int category_index, int animation_index, const int keyframe_index) const;
	void set_keyframe_animation_keyframe_index(const int category_index, int animation_index, const int keyframe_index, int value);

	int get_keyframe_next_keyframe_index(const int category_index, const int animation_index, const int keyframe_index) const;
	void set_keyframe_next_keyframe_index(const int category_index, const int animation_index, const int keyframe_index, const int value);

	Ref<Curve> get_keyframe_in_curve(const int category_index, const int animation_index, const int keyframe_index) const;
	void set_keyframe_in_curve(const int category_index, const int animation_index, const int keyframe_index, const Ref<Curve> &value);

	Vector2 get_keyframe_node_position(const int category_index, int animation_index, const int keyframe_index) const;
	void set_keyframe_node_position(const int category_index, const int animation_index, const int keyframe_index, const Vector2 &value);

	void initialize();
	void load_keyframe_data(int keyframe_index);

	ProceduralAnimation();
	~ProceduralAnimation();

protected:
	_FORCE_INLINE_ TransformAnimationKey _interpolate(const TransformAnimationKey &p_a, const TransformAnimationKey &p_b, float p_c) const;

	_FORCE_INLINE_ Vector3 _interpolate(const Vector3 &p_a, const Vector3 &p_b, float p_c) const;
	_FORCE_INLINE_ Quat _interpolate(const Quat &p_a, const Quat &p_b, float p_c) const;
	_FORCE_INLINE_ Variant _interpolate(const Variant &p_a, const Variant &p_b, float p_c) const;
	_FORCE_INLINE_ float _interpolate(const float &p_a, const float &p_b, float p_c) const;

	_FORCE_INLINE_ TransformAnimationKey _cubic_interpolate(const TransformAnimationKey &p_pre_a, const TransformAnimationKey &p_a, const TransformAnimationKey &p_b, const TransformAnimationKey &p_post_b, float p_c) const;
	_FORCE_INLINE_ Vector3 _cubic_interpolate(const Vector3 &p_pre_a, const Vector3 &p_a, const Vector3 &p_b, const Vector3 &p_post_b, float p_c) const;
	_FORCE_INLINE_ Quat _cubic_interpolate(const Quat &p_pre_a, const Quat &p_a, const Quat &p_b, const Quat &p_post_b, float p_c) const;
	_FORCE_INLINE_ Variant _cubic_interpolate(const Variant &p_pre_a, const Variant &p_a, const Variant &p_b, const Variant &p_post_b, float p_c) const;
	_FORCE_INLINE_ float _cubic_interpolate(const float &p_pre_a, const float &p_a, const float &p_b, const float &p_post_b, float p_c) const;

	template <class T>
	_FORCE_INLINE_ T _interpolate(const Vector<AnimationKey> &p_keys, float p_time, KeyInterpolationType p_interp, bool p_loop_wrap, bool *p_ok) const;

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

	Map<int, Category *> _categories;
	Map<int, Vector<AnimationKey> *> _animation_data;

	Ref<Animation> _animation;
	Map<int, String> _keyframe_names;
};

#endif