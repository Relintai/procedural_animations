#ifndef PROCEDURAL_ANIMATION_EDITOR_PLUGIN_H
#define PROCEDURAL_ANIMATION_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "editor/pane_drag.h"

#include "scene/gui/graph_edit.h"
#include "scene/gui/menu_button.h"

#include "procedural_animation.h"

class ProceduralAnimationEditor : public VBoxContainer {
	GDCLASS(ProceduralAnimationEditor, VBoxContainer);

public:
	void edit(const Ref<ProceduralAnimation> &animation);
	void add_frame_button_pressed();

	ProceduralAnimationEditor() {}
	ProceduralAnimationEditor(EditorNode *p_editor);
	~ProceduralAnimationEditor();

protected:
	static void _bind_methods();

private:
	int _editor_category;
	int _editor_category_animation;

	ConfirmationDialog *_name_popuop;
	Label *_name_pupup_label;
	LineEdit *_name_popup_line_edit;

	GraphNode *_start_node;
	Ref<ProceduralAnimation> _animation;
	GraphEdit *_graph_edit;
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
