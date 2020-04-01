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

#ifndef PROCEDURAL_ANIMATION_EDITOR_PLUGIN_H
#define PROCEDURAL_ANIMATION_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "editor/pane_drag.h"

#include "scene/gui/graph_edit.h"
#include "scene/gui/menu_button.h"

#include "core/core_string_names.h"
#include "procedural_animation.h"

#include "editor/plugins/curve_editor_plugin.h"

class ProceduralAnimationEditor : public VBoxContainer {
	GDCLASS(ProceduralAnimationEditor, VBoxContainer);

public:
	enum DeletePopupActions {
		DELETE_POPUP_KEYFRAME,
	};

public:
	void edit(const Ref<ProceduralAnimation> &animation);

	void add_frame_button_pressed();

	void load_animation();
	void clear_keyframe_nodes();

	void on_keyframe_node_changed(Node *node);

	void on_delete_popup_confirmed();

	void on_connection_request(const String &from, const int from_slot, const String &to, const int to_slot);
	void on_disconnection_request(const String &from, const int from_slot, const String &to, const int to_slot);

	ProceduralAnimationEditor();
	ProceduralAnimationEditor(EditorNode *p_editor);
	~ProceduralAnimationEditor();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	DeletePopupActions _delete_popup_action;
	ConfirmationDialog *_delete_popuop;

	ConfirmationDialog *_name_popuop;
	Label *_name_pupup_label;
	LineEdit *_name_popup_line_edit;

	GraphNode *_start_node;
	Ref<ProceduralAnimation> _animation;
	GraphEdit *_graph_edit;

	ToolButton *_pin;
};

class ProceduralAnimationEditorGraphNode : public GraphNode {
	GDCLASS(ProceduralAnimationEditorGraphNode, GraphNode);

public:
	int get_id() const;
	void set_id(const int id);

	String get_keyframe_name() const;
	void set_keyframe_name(const String &value);
	void on_keyframe_name_modified(const String &value);

	int get_animation_keyframe_index() const;
	void set_animation_keyframe_index(const int value);

	int get_next_keyframe() const;
	void set_next_keyframe(const int value);

	//Ref<Curve> get_in_curve() const;
	//void set_in_curve(const Ref<Curve> &value);

	float get_transition() const;
	void set_transition(const float value);

	Ref<ProceduralAnimation> get_animation();
	void set_animation(const Ref<ProceduralAnimation> &animation);

	ProceduralAnimationEditorGraphNode();
	~ProceduralAnimationEditorGraphNode();

protected:
	void on_animation_keyframe_spinbox_value_changed(float value);
	void on_dragged(Vector2 from, Vector2 to);

	void changed();

	void _notification(int p_what);
	static void _bind_methods();

private:
	int _id;
	LineEdit *_name;
	SpinBox *_animation_keyframe_spinbox;

	int _animation_keyframe_index;
	int _next_keyframe;
	CurveEditor *_curve_editor;
	//Ref<Curve> _in_curve;
	float _transition;

	Ref<ProceduralAnimation> _animation;
};

class ProceduralAnimationEditorPlugin : public EditorPlugin {
	GDCLASS(ProceduralAnimationEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "ProceduralAnimationEditor"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	ProceduralAnimationEditorPlugin(EditorNode *p_node);
	~ProceduralAnimationEditorPlugin();

protected:
private:
	ProceduralAnimationEditor *animation_editor;
	ToolButton *animation_editor_button;
	EditorNode *editor;
};

#endif
