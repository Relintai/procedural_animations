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

#include "procedural_animation_editor_plugin.h"

#include "editor/editor_scale.h"

#include "core/math/math_defs.h"

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
	_animation_player = NULL;
	_selected_category = -1;
	_selected_animation = -1;

	set_h_size_flags(SIZE_EXPAND_FILL);

	//top bar
	HBoxContainer *hbc = memnew(HBoxContainer);
	add_child(hbc);

	_category_option_button = memnew(OptionButton);
	_category_option_button->set_h_size_flags(SIZE_EXPAND_FILL);
	_category_option_button->connect("item_selected", this, "on_category_option_button_pressed");
	hbc->add_child(_category_option_button);

	_animation_option_button = memnew(OptionButton);
	_animation_option_button->set_h_size_flags(SIZE_EXPAND_FILL);
	_animation_option_button->connect("item_selected", this, "on_animation_option_button_pressed");
	hbc->add_child(_animation_option_button);

	_play_bw_from = memnew(ToolButton);
	_play_bw_from->set_tooltip(TTR("Play selected animation backwards from current pos. (A)"));
	hbc->add_child(_play_bw_from);

	_play_bw = memnew(ToolButton);
	_play_bw->set_tooltip(TTR("Play selected animation backwards from end. (Shift+A)"));
	hbc->add_child(_play_bw);

	_stop = memnew(ToolButton);
	_stop->set_toggle_mode(true);
	hbc->add_child(_stop);
	_stop->set_tooltip(TTR("Stop animation playback. (S)"));

	_play = memnew(ToolButton);
	_play->set_tooltip(TTR("Play selected animation from start. (Shift+D)"));
	hbc->add_child(_play);

	_play_from = memnew(ToolButton);
	_play_from->set_tooltip(TTR("Play selected animation from current pos. (D)"));
	hbc->add_child(_play_from);

	_pin = memnew(ToolButton);
	_pin->set_toggle_mode(true);
	_pin->set_tooltip(TTR("Pin"));
	hbc->add_child(_pin);

	Control *spacer = memnew(Control);
	spacer->set_custom_minimum_size(Size2(0, 2 * EDSCALE));
	add_child(spacer);

	//bottom
	_animation_editor = memnew(ProceduralAnimationEditor(p_editor));
	//_animation_editor->set_margin(Margin::MARGIN_TOP, 8 * EDSCALE);
	_animation_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(_animation_editor);
}

ProceduralAnimationPlayerEditor::~ProceduralAnimationPlayerEditor() {
}

void ProceduralAnimationPlayerEditor::_notification(int p_what) {

	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED: {
			_play->set_icon(get_icon("PlayStart", "EditorIcons"));
			_play_from->set_icon(get_icon("Play", "EditorIcons"));
			_play_bw->set_icon(get_icon("PlayStartBackwards", "EditorIcons"));
			_play_bw_from->set_icon(get_icon("PlayBackwards", "EditorIcons"));

			_stop->set_icon(get_icon("Stop", "EditorIcons"));

			_pin->set_icon(get_icon("Pin", "EditorIcons"));
		} break;
	}
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
