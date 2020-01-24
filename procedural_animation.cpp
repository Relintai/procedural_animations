#include "procedural_animation.h"

StringName ProceduralAnimation::get_editor_category() const {
	if (_editor_category >= 0 && _editor_category < _categories.size()) {
		return _categories.get(_editor_category)->name;
	}

	return "---";
}
void ProceduralAnimation::set_editor_category(const StringName &value) {
	for (int i = 0; i < _categories.size(); ++i) {
		if (_categories.get(i)->name == value) {
			_editor_category = i;
			return;
		}
	}

	_editor_category = -1;
	_editor_category_animation = -1;
}

StringName ProceduralAnimation::get_editor_category_animation() const {
	if (_editor_category >= 0 && _editor_category < _categories.size()) {
		Category *category = _categories.get(_editor_category);

		if (_editor_category_animation >= 0 && _editor_category_animation < category->animations.size()) {
			return category->animations.get(_editor_category_animation)->name;
		}
	}

	return "---";
}
void ProceduralAnimation::set_editor_category_animation(const StringName &value) {
	if (_editor_category >= 0 && _editor_category < _categories.size()) {
		Category *category = _categories.get(_editor_category);

		for (int i = 0; i < category->animations.size(); ++i) {
			if (category->animations.get(i)->name == value) {
				_editor_category_animation = i;
				return;
			}
		}
	}

	_editor_category_animation = -1;
}

String ProceduralAnimation::get_add_editor_category_name() const {
	return _editor_add_category_name;
}
void ProceduralAnimation::set_add_editor_category_name(const String &value) {
	_editor_add_category_name = value;
}

bool ProceduralAnimation::get_add_editor_category() const {
	return false;
}
void ProceduralAnimation::set_add_editor_category(const bool value) {
	Category *cat = memnew(Category);
	cat->name = _editor_add_category_name;
	_categories.push_back(cat);

	_editor_category = _categories.size() - 1;
}

String ProceduralAnimation::get_add_editor_category_animation_name() const {
	return _add_editor_category_animation_name;
}
void ProceduralAnimation::set_add_editor_category_animation_name(const String &value) {
	_add_editor_category_animation_name = value;
}

bool ProceduralAnimation::get_add_editor_animation_category() const {
	return false;
}
void ProceduralAnimation::set_add_editor_animation_category(const bool value) {
	if (_editor_category >= 0 && _editor_category < _categories.size()) {
		AnimationEntry *entry = memnew(AnimationEntry);
		entry->name = _add_editor_category_animation_name;
		_categories.get(_editor_category)->animations.push_back(entry);

		_editor_category_animation = _categories.get(_editor_category)->animations.size() - 1;
	}
}

ProceduralAnimation::ProceduralAnimation() {
	_editor_category = -1;
	_editor_category_animation = -1;
}

ProceduralAnimation::~ProceduralAnimation() {
	for (int i = 0; i < _categories.size(); ++i) {
		Category *categ = _categories.get(i);

		for (int j = 0; j < categ->animations.size(); ++j) {
			memdelete(categ->animations.get(j));
		}

		memdelete(categ);
	}

	_categories.clear();

	_animation.unref();
}

bool ProceduralAnimation::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("categories|")) {

		int category_index = name.get_slicec('|', 1).to_int();
		String what = name.get_slicec('|', 2);

		ERR_FAIL_INDEX_V(category_index, _categories.size(), false);

		Category *cat = _categories.get(category_index);

		if (what == "name") {
			cat->name = p_value;

			return true;
		} else if (what == "animation") {
			int animation_index = name.get_slicec('|', 3).to_int();
			String anim_prop_name = name.get_slicec('|', 4);

			Category *category = _categories.get(category_index);

			ERR_FAIL_INDEX_V(category_index, _categories.get(category_index)->animations.size(), false);

			if (anim_prop_name == "name") {
				category->animations.get(animation_index)->name = p_value;

				return true;
			} else if (anim_prop_name == "index") {
				category->animations.get(animation_index)->index = p_value;

				return true;
			} else if (anim_prop_name == "keyframe_index") {
				category->animations.get(animation_index)->keyframe_index = p_value;

				return true;
			} else if (anim_prop_name == "next_animation") {
				category->animations.get(animation_index)->next_animation = p_value;

				return true;
			} else if (anim_prop_name == "in_curve") {
				category->animations.get(animation_index)->in_curve = p_value;

				return true;
			} else {
				return false;
			}
		}

		return false;
	} else {
		return false;
	}

	return true;
}

bool ProceduralAnimation::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("categories|")) {

		int category_index = name.get_slicec('|', 1).to_int();
		String what = name.get_slicec('|', 2);

		ERR_FAIL_INDEX_V(category_index, _categories.size(), false);

		if (what == "name") {
			r_ret = _categories.get(category_index)->name;

			return true;
		} else if (what == "animation") {
			int animation_index = name.get_slicec('|', 3).to_int();
			String anim_prop_name = name.get_slicec('|', 4);

			Category *category = _categories.get(category_index);

			ERR_FAIL_INDEX_V(animation_index, _categories.get(category_index)->animations.size(), false);

			if (anim_prop_name == "name") {
				r_ret = category->name;

				return true;
			} else if (anim_prop_name == "index") {
				r_ret = category->animations.get(animation_index)->index;

				return true;
			} else if (anim_prop_name == "keyframe_index") {
				r_ret = category->animations.get(animation_index)->keyframe_index;

				return true;
			} else if (anim_prop_name == "next_animation") {
				r_ret = category->animations.get(animation_index)->next_animation;

				return true;
			} else if (anim_prop_name == "in_curve") {
				r_ret = category->animations.get(animation_index)->in_curve;

				return true;
			} else {
				return false;
			}
		}

		return false;
	} else {
		return false;
	}

	return true;
}

void ProceduralAnimation::_validate_property(PropertyInfo &property) const {
	String name = property.name;

	if (name == "editor_category") {
		String names("---");
		for (int i = 0; i < _categories.size(); ++i) {
			names += ",";

			names += _categories.get(i)->name;
		}

		property.hint_string = names;
	} else if (name == "editor_category_animation") {
		String names("---");

		if (_editor_category >= 0 && _editor_category < _categories.size()) {
			Category *category = _categories.get(_editor_category);

			for (int i = 0; i < category->animations.size(); ++i) {
				names += ",";

				names += category->animations.get(i)->name;
			}
		}

		property.hint_string = names;
	}
}

void ProceduralAnimation::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _categories.size(); ++i) {
		int property_usange_editor = PROPERTY_USAGE_NOEDITOR;

		if (i == _editor_category) {
			property_usange_editor = PROPERTY_USAGE_EDITOR;
		}

		Category *categ = _categories.get(i);

		p_list->push_back(PropertyInfo(Variant::STRING, "categories|" + itos(i) + "|name", PROPERTY_HINT_NONE, "", property_usange_editor | PROPERTY_USAGE_INTERNAL | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED));

		for (int j = 0; j < categ->animations.size(); ++j) {
			if (i == _editor_category && j == _editor_category_animation) {
				property_usange_editor = PROPERTY_USAGE_EDITOR;
			} else {
				property_usange_editor = PROPERTY_USAGE_NOEDITOR;
			}

			p_list->push_back(PropertyInfo(Variant::STRING, "categories|" + itos(i) + "|animation|" + itos(j) + "|name", PROPERTY_HINT_NONE, "", property_usange_editor | PROPERTY_USAGE_INTERNAL | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED));
			p_list->push_back(PropertyInfo(Variant::INT, "categories|" + itos(i) + "|animation|" + itos(j) + "|index", PROPERTY_HINT_NONE, "", property_usange_editor | PROPERTY_USAGE_INTERNAL));
			p_list->push_back(PropertyInfo(Variant::INT, "categories|" + itos(i) + "|animation|" + itos(j) + "|keyframe_index", PROPERTY_HINT_NONE, "", property_usange_editor | PROPERTY_USAGE_INTERNAL));
			p_list->push_back(PropertyInfo(Variant::STRING, "categories|" + itos(i) + "|animation|" + itos(j) + "|next_animation", PROPERTY_HINT_NONE, "", property_usange_editor | PROPERTY_USAGE_INTERNAL));
			p_list->push_back(PropertyInfo(Variant::OBJECT, "categories|" + itos(i) + "|animation|" + itos(j) + "|in_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve", property_usange_editor | PROPERTY_USAGE_INTERNAL));
		}
	}
}

void ProceduralAnimation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_editor_category"), &ProceduralAnimation::get_editor_category);
	ClassDB::bind_method(D_METHOD("set_editor_category", "value"), &ProceduralAnimation::set_editor_category);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "editor_category", PROPERTY_HINT_ENUM, "---", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_editor_category", "get_editor_category");

	ClassDB::bind_method(D_METHOD("get_editor_category_animation"), &ProceduralAnimation::get_editor_category_animation);
	ClassDB::bind_method(D_METHOD("set_editor_category_animation", "value"), &ProceduralAnimation::set_editor_category_animation);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "editor_category_animation", PROPERTY_HINT_ENUM, "---", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_editor_category_animation", "get_editor_category_animation");

	ClassDB::bind_method(D_METHOD("get_add_editor_category_name"), &ProceduralAnimation::get_add_editor_category_name);
	ClassDB::bind_method(D_METHOD("set_add_editor_category_name", "value"), &ProceduralAnimation::set_add_editor_category_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "add_editor_category_name"), "set_add_editor_category_name", "get_add_editor_category_name");

	ClassDB::bind_method(D_METHOD("get_add_editor_category"), &ProceduralAnimation::get_add_editor_category);
	ClassDB::bind_method(D_METHOD("set_add_editor_category", "value"), &ProceduralAnimation::set_add_editor_category);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "add_editor_category", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_add_editor_category", "get_add_editor_category");

	ClassDB::bind_method(D_METHOD("get_add_editor_category_animation_name"), &ProceduralAnimation::get_add_editor_category_animation_name);
	ClassDB::bind_method(D_METHOD("set_add_editor_category_animation_name", "value"), &ProceduralAnimation::set_add_editor_category_animation_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "add_editor_category_animation_name"), "set_add_editor_category_animation_name", "get_add_editor_category_animation_name");

	ClassDB::bind_method(D_METHOD("get_add_editor_animation_category"), &ProceduralAnimation::get_add_editor_animation_category);
	ClassDB::bind_method(D_METHOD("set_add_editor_animation_category", "value"), &ProceduralAnimation::set_add_editor_animation_category);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "add_editor_animation_category", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_add_editor_animation_category", "get_add_editor_animation_category");
}