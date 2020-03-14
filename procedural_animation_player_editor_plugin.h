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

#ifndef PROCEDURAL_ANIMATION_PLAYER_EDITOR_PLUGIN_H
#define PROCEDURAL_ANIMATION_PLAYER_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/gui/box_container.h"

#include "core/core_string_names.h"
#include "procedural_animation_player.h"

#include "scene/gui/menu_button.h"

class ProceduralAnimationEditor;

class ProceduralAnimationPlayerEditor : public VBoxContainer {
	GDCLASS(ProceduralAnimationPlayerEditor, VBoxContainer);

public:
	void edit(ProceduralAnimationPlayer *player);

	ProceduralAnimationPlayerEditor();
	ProceduralAnimationPlayerEditor(EditorNode *p_editor);
	~ProceduralAnimationPlayerEditor();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	int _selected_category;
	int _selected_animation;

	OptionButton *_category_option_button;
	OptionButton *_animation_option_button;

	Button *_stop;
	Button *_play;
	Button *_play_from;
	Button *_play_bw;
	Button *_play_bw_from;
	ToolButton *_pin;

	ProceduralAnimationEditor *_animation_editor;

	ProceduralAnimationPlayer *_animation_player;
};

class ProceduralAnimationPlayerEditorPlugin : public EditorPlugin {
	GDCLASS(ProceduralAnimationPlayerEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "ProceduralAnimationPlayerEditor"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	ProceduralAnimationPlayerEditorPlugin(EditorNode *p_node);
	~ProceduralAnimationPlayerEditorPlugin();

protected:
private:
	ProceduralAnimationPlayerEditor *animation_editor;
	ToolButton *animation_editor_button;
	EditorNode *editor;
};

#endif
