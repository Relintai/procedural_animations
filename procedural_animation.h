#ifndef PROCEDURAL_ANIMATION_H
#define PROCEDURAL_ANIMATION_H

#include "core/resource.h"

#include "core/vector.h"
#include "scene/resources/animation.h"
#include "scene/resources/curve.h"

#include "scene/resources/animation.h"

class ProceduralAnimation : public Resource {
	GDCLASS(ProceduralAnimation, Resource);

public:
	StringName get_editor_category() const;
	void set_editor_category(const StringName &value);

	StringName get_editor_category_animation() const;
	void set_editor_category_animation(const StringName &value);

	String get_add_editor_category_name() const;
	void set_add_editor_category_name(const String &value);

	bool get_add_editor_category() const;
	void set_add_editor_category(const bool value);

	String get_add_editor_category_animation_name() const;
	void set_add_editor_category_animation_name(const String &value);

	bool get_add_editor_animation_category() const;
	void set_add_editor_animation_category(const bool value);

	ProceduralAnimation();
	~ProceduralAnimation();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	virtual void _validate_property(PropertyInfo &property) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

protected:
	struct AnimationEntry {
		StringName name;
		int index;
		int keyframe_index;
		StringName next_animation;
		Ref<Curve> in_curve;

		AnimationEntry() {
			index = 0;
			keyframe_index = 0;
		}
	};

	struct Category {
		StringName name;
		Vector<AnimationEntry *> animations;
	};

private:
	int _editor_category;
	int _editor_category_animation;

	String _editor_add_category_name;
	String _add_editor_category_animation_name;

	Vector<Category *> _categories;

	Ref<Animation> _animation;
};

#endif