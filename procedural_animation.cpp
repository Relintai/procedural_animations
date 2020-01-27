#include "procedural_animation.h"

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

int ProceduralAnimation::get_start_frame_index(const int category_index, const int animation_index) const {
	ERR_FAIL_COND_V(!_categories.has(category_index), 0);

	Category *cat = _categories[category_index];

	ERR_FAIL_COND_V(!cat->animations.has(animation_index), 0);

	return cat->animations[animation_index]->start_frame_index;
}
void ProceduralAnimation::set_start_frame_index(const int category_index, const int animation_index, const int value) {
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

	AnimationEntry *ae = cat->animations[category_index];

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

	AnimationEntry *ae = cat->animations[category_index];

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

	AnimationEntry *ae = cat->animations[category_index];

	ERR_FAIL_COND(!ae->keyframes.has(keyframe_index));

	AnimationKeyFrame *entry = ae->keyframes[keyframe_index];

	cat->animations.erase(keyframe_index);

	memdelete(entry);
}

String ProceduralAnimation::get_keyframe_name(const int category_index, const int animation_index, const int keyframe_index) {
	return "";
}
void ProceduralAnimation::set_keyframe_name(const int category_index, const int animation_index, const int keyframe_index, const String &value) {

}

int ProceduralAnimation::get_keyframe_animation_keyframe_index(const int category_index, int animation_index, const int keyframe_index) const {
	return 0;
}
void ProceduralAnimation::set_keyframe_animation_keyframe_index(const int category_index, int animation_index, const int keyframe_index, int value) {

}

int ProceduralAnimation::get_keyframe_next_keyframe_index(const int category_index, const int animation_index, const int keyframe_index) const {
	return 0;
}
void ProceduralAnimation::set_keyframe_next_keyframe_index(const int category_index, const int animation_index, const int keyframe_index, const int value) {

}

Ref<Curve> ProceduralAnimation::get_keyframe_in_curve(const int category_index, const int animation_index, const int keyframe_index) const {
	return Ref<Curve>();
}
void ProceduralAnimation::set_keyframe_in_curve(const int category_index, const int animation_index, const int keyframe_index, const Ref<Curve> &value) {

}

Vector2 ProceduralAnimation::get_keyframe_node_position(const int category_index, int animation_index, const int keyframe_index) const {
	return Vector2();
}
void ProceduralAnimation::set_keyframe_node_position(const int category_index, const int animation_index, const int keyframe_index, const Vector2 &value) {

}


ProceduralAnimation::ProceduralAnimation() {
}

ProceduralAnimation::~ProceduralAnimation() {
	for (Map<int, Category *>::Element *E = _categories.front(); E; E = E->next()) {
		memdelete(E->get());
	}

	_categories.clear();

	_animation.unref();
}

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

			if (!cat->animations.has(category_index)) {
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

				if (!ae->keyframes.has(category_index)) {
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
	int property_usange = PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL;

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
				p_list->push_back(PropertyInfo(Variant::INT, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/next_animation", PROPERTY_HINT_NONE, "", property_usange));
				p_list->push_back(PropertyInfo(Variant::OBJECT, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/in_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve", property_usange));
				p_list->push_back(PropertyInfo(Variant::VECTOR2, "categories/" + itos(E->key()) + "/animation/" + itos(A->key()) + "/keyframe/" + itos(K->key()) + "/position", PROPERTY_HINT_NONE, "", property_usange));
			}
		}
	}
}

void ProceduralAnimation::_bind_methods() {
/*
	ClassDB::bind_method(D_METHOD("get_editor_category"), &ProceduralAnimation::get_editor_category);
	ClassDB::bind_method(D_METHOD("set_editor_category", "value"), &ProceduralAnimation::set_editor_category);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "editor_category", PROPERTY_HINT_ENUM, "---", PROPERTY_USAGE_EDITOR / PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_editor_category", "get_editor_category");

	ClassDB::bind_method(D_METHOD("get_editor_category_animation"), &ProceduralAnimation::get_editor_category_animation);
	ClassDB::bind_method(D_METHOD("set_editor_category_animation", "value"), &ProceduralAnimation::set_editor_category_animation);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "editor_category_animation", PROPERTY_HINT_ENUM, "---", PROPERTY_USAGE_EDITOR / PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_editor_category_animation", "get_editor_category_animation");

	ClassDB::bind_method(D_METHOD("get_add_editor_category_name"), &ProceduralAnimation::get_add_editor_category_name);
	ClassDB::bind_method(D_METHOD("set_add_editor_category_name", "value"), &ProceduralAnimation::set_add_editor_category_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "add_editor_category_name"), "set_add_editor_category_name", "get_add_editor_category_name");

	ClassDB::bind_method(D_METHOD("get_add_editor_category"), &ProceduralAnimation::get_add_editor_category);
	ClassDB::bind_method(D_METHOD("set_add_editor_category", "value"), &ProceduralAnimation::set_add_editor_category);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "add_editor_category", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR / PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_add_editor_category", "get_add_editor_category");

	ClassDB::bind_method(D_METHOD("get_add_editor_category_animation_name"), &ProceduralAnimation::get_add_editor_category_animation_name);
	ClassDB::bind_method(D_METHOD("set_add_editor_category_animation_name", "value"), &ProceduralAnimation::set_add_editor_category_animation_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "add_editor_category_animation_name"), "set_add_editor_category_animation_name", "get_add_editor_category_animation_name");

	ClassDB::bind_method(D_METHOD("get_add_editor_animation_category"), &ProceduralAnimation::get_add_editor_animation_category);
	ClassDB::bind_method(D_METHOD("set_add_editor_animation_category", "value"), &ProceduralAnimation::set_add_editor_animation_category);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "add_editor_animation_category", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR / PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_add_editor_animation_category", "get_add_editor_animation_category");
*/
}