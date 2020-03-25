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

	void set_length(float p_length);
	float get_length() const;

	void set_loop(bool p_enabled);
	bool has_loop() const;

	//Keyframes
	PoolVector<int> get_keyframe_indices() const;
	int add_keyframe();
	void remove_keyframe(const int keyframe_index);
	bool has_keyframe(const int keyframe_index) const;

	String get_keyframe_name(const int keyframe_index) const;
	void set_keyframe_name(const int keyframe_index, const String &value);

	int get_keyframe_animation_keyframe_index(const int keyframe_index) const;
	void set_keyframe_animation_keyframe_index(const int keyframe_index, int value);

	int get_keyframe_next_keyframe_index(const int keyframe_index) const;
	void set_keyframe_next_keyframe_index(const int keyframe_index, const int value);

	Ref<Curve> get_keyframe_in_curve(const int keyframe_index) const;
	void set_keyframe_in_curve(const int keyframe_index, const Ref<Curve> &value);

	Vector2 get_keyframe_node_position(const int keyframe_index) const;
	void set_keyframe_node_position(const int keyframe_index, const Vector2 &value);

	void initialize();
	void load_keyframe_data(int keyframe_index);

	float track_get_key_time(int p_track, int p_key_idx) const;
	float track_get_key_transition(int p_track, int p_key_idx) const;
	Variant track_get_key_value(int p_track, int p_key_idx) const;

	void get_key_indices(int p_track, float p_time, float p_delta, List<int> *p_indices) const;
	void track_get_key_indices_in_range(int p_track, float p_time, float p_delta, List<int> *p_indices) const;

	int track_find_key(int p_track, float p_time, bool p_exact = false) const;

	Vector<Variant> method_track_get_params(int p_track, int p_key_idx) const;
	StringName method_track_get_name(int p_track, int p_key_idx) const;

	RES audio_track_get_key_stream(int p_track, int p_key) const;
	float audio_track_get_key_start_offset(int p_track, int p_key) const;
	float audio_track_get_key_end_offset(int p_track, int p_key) const;

	StringName animation_track_get_key_animation(int p_track, int p_key) const;

	//Interpolations
	Error transform_track_interpolate(int p_track, float p_time, Vector3 *r_loc, Quat *r_rot, Vector3 *r_scale) const;
	Variant value_track_interpolate(int p_track, float p_time) const;
	float bezier_track_interpolate(int p_track, float p_time) const;

	//Animation forwards
	int get_track_count() const;
	Animation::TrackType track_get_type(int p_track) const;

	NodePath track_get_path(int p_track) const;
	int find_track(const NodePath &p_path) const;

	bool track_is_enabled(int p_track) const;

	int track_get_key_count(int p_track) const;

	Animation::UpdateMode value_track_get_update_mode(int p_track) const;

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
	float _length;
	bool _loop;

	String _editor_add_category_name;
	String _add_editor_category_animation_name;

	Vector2 _start_node_position;
	int _start_frame_index;
	Map<int, AnimationKeyFrame *> _keyframes;

	Map<int, Vector<AnimationKey> *> _animation_data;

	Ref<Animation> _animation;
	Map<int, String> _keyframe_names;
};

#endif