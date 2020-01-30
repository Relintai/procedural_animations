#ifndef PROCEDURAL_ANIMATION_H
#define PROCEDURAL_ANIMATION_H

#include "core/resource.h"

#include "core/map.h"
#include "core/vector.h"
#include "core/pool_vector.h"
#include "core/math/vector2.h"
#include "scene/resources/animation.h"
#include "scene/resources/curve.h"

#include "scene/resources/animation.h"

class ProceduralAnimation : public Resource {
	GDCLASS(ProceduralAnimation, Resource);

public:
	Ref<Animation> get_animation() const;
	void set_animation(const Ref<Animation> &value);

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

	ProceduralAnimation();
	~ProceduralAnimation();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

protected:
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

private:
	String _editor_add_category_name;
	String _add_editor_category_animation_name;

	Map<int, Category *> _categories;

	Ref<Animation> _animation;
	Map<int, String> _keyframe_names;
};

#endif