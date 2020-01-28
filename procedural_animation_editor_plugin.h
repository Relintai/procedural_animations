#ifndef PROCEDURAL_ANIMATION_EDITOR_PLUGIN_H
#define PROCEDURAL_ANIMATION_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "editor/pane_drag.h"

#include "scene/gui/graph_edit.h"
#include "scene/gui/menu_button.h"

#include "procedural_animation.h"
#include "core/core_string_names.h"

#include "editor/plugins/curve_editor_plugin.h"

class ProceduralAnimationEditor : public VBoxContainer {
	GDCLASS(ProceduralAnimationEditor, VBoxContainer);

public:
	enum NamePopupActions {
		NAME_POPUP_ADD_CATEGORY_NAME,
		NAME_POPUP_EDIT_CATEGORY_NAME,
		NAME_POPUP_ADD_ANIMATION_NAME,
		NAME_POPUP_EDIT_ANIMATION_NAME,
	};

	enum DeletePopupActions {
		DELETE_POPUP_CATEGORY,
		DELETE_POPUP_ANIMATION,
		DELETE_POPUP_KEYFRAME,
	};

public:
	void edit(const Ref<ProceduralAnimation> &animation);
	void add_frame_button_pressed();

	void load_selected_animation();
	void clear_keyframe_nodes();

	void on_animation_option_button_pressed(int indx);
	void on_category_option_button_pressed(int indx);
	void refresh_option_buttons();
	void refresh_animation_option_button();
	void on_keyframe_node_changed(Node *node);

	void category_tool_button_id_pressed(int id);
	void animation_tool_button_id_pressed(int id);
	void show_name_popup(NamePopupActions action);
	void on_name_popup_confirmed();
	void on_delete_popup_confirmed();

	ProceduralAnimationEditor();
	ProceduralAnimationEditor(EditorNode *p_editor);
	~ProceduralAnimationEditor();

protected:
	static void _bind_methods();

private:
	int _selected_category;
	int _selected_animation;

	OptionButton *_category_option_button;
	OptionButton *_animation_option_button;

	DeletePopupActions _delete_popup_action;
	ConfirmationDialog *_delete_popuop;

	NamePopupActions _name_popup_action;
	ConfirmationDialog *_name_popuop;
	Label *_name_pupup_label;
	LineEdit *_name_popup_line_edit;

	GraphNode *_start_node;
	Ref<ProceduralAnimation> _animation;
	GraphEdit *_graph_edit;
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

	Ref<Curve> get_in_curve() const;
	void set_in_curve(const Ref<Curve> &value);

	void set_animation_keyframe_names(const PoolVector<String> &names);

	void on_animation_keyframe_spinbox_value_changed(const String &value);
	void changed();
	
	ProceduralAnimationEditorGraphNode();
	~ProceduralAnimationEditorGraphNode();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	int _id;
	LineEdit *_name;
	//OptionButton *_animation_keyframe_index_option_button;
	SpinBox *_animation_keyframe_spinbox;
	int _animation_keyframe_index;
	int _next_keyframe;
	CurveEditor *_curve_editor;
	Ref<Curve> _in_curve;
};

class ProceduralAnimationEditorPlugin : public EditorPlugin {
	GDCLASS(ProceduralAnimationEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "ProceduralAnimation"; }
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
