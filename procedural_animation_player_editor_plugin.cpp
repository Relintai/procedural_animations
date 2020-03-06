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

#include "procedural_animation_player_editor_plugin.h"

#include "editor/editor_scale.h"

void ProceduralAnimationPlayerEditor::edit(ProceduralAnimationPlayer *player) {
	_animation_player = player;

	_selected_category = -1;
	_selected_animation = -1;
}

void ProceduralAnimationPlayerEditor::_bind_methods() {
}

ProceduralAnimationPlayerEditor::ProceduralAnimationPlayerEditor() {
	_animation_player = NULL;
	_selected_category = -1;
	_selected_animation = -1;
}

ProceduralAnimationPlayerEditor::ProceduralAnimationPlayerEditor(EditorNode *p_editor) {
	set_h_size_flags(SIZE_EXPAND_FILL);

	_animation_player = NULL;
	_selected_category = -1;
	_selected_animation = -1;
}

ProceduralAnimationPlayerEditor::~ProceduralAnimationPlayerEditor() {
}

void ProceduralAnimationPlayerEditorPlugin::edit(Object *p_object) {
	if (Object::cast_to<ProceduralAnimationPlayer>(p_object)) {
		animation_editor->edit(Object::cast_to<ProceduralAnimationPlayer>(p_object));
		animation_editor_button->show();
	} else
		animation_editor_button->hide();
}

bool ProceduralAnimationPlayerEditorPlugin::handles(Object *p_object) const {

	bool handles = p_object->is_class("ProceduralAnimationPlayer");

	if (handles) {
		animation_editor_button->show();
	} else {
		animation_editor_button->hide();
	}

	return handles;
}

void ProceduralAnimationPlayerEditorPlugin::make_visible(bool p_visible) {
}

ProceduralAnimationPlayerEditorPlugin::ProceduralAnimationPlayerEditorPlugin(EditorNode *p_node) {

	editor = p_node;

	animation_editor = memnew(ProceduralAnimationPlayerEditor(editor));

	animation_editor_button = add_control_to_bottom_panel(animation_editor, "Procedural Animations");

	animation_editor->hide();
}

ProceduralAnimationPlayerEditorPlugin::~ProceduralAnimationPlayerEditorPlugin() {
}
