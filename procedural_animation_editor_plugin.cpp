#include "procedural_animation_editor_plugin.h"

#include "editor/editor_scale.h"


void ProceduralAnimationEditor::edit(const Ref<ProceduralAnimation> &animation) {
	_animation = animation;
}

void ProceduralAnimationEditor::add_frame_button_pressed() {
	GraphNode *gn = memnew(GraphNode);
	gn->set_title("Animation Frame");
	gn->set_show_close_button(true);
	//gn->set_position()

	Label *l1 = memnew(Label);
	l1->set_text("Name");
	gn->add_child(l1);

	LineEdit *le = memnew(LineEdit);
	gn->add_child(le);

	Label *l2 = memnew(Label);
	l2->set_text("Keyframe");
	gn->add_child(l2);

	HBoxContainer *kfc = memnew(HBoxContainer);
	gn->add_child(kfc);

	OptionButton *kob = memnew(OptionButton);
	kfc->add_child(kob);

	Button *kb = memnew(Button);
	kb->set_text("Edit name");
	kfc->add_child(kb);

	Label *l3 = memnew(Label);
	l3->set_text("In Curve");
	gn->add_child(l3);

	//placeholder
	Button *pb = memnew(Button);
	pb->set_text("Edit / Show");
	pb->set_custom_minimum_size(Size2(0, 69) * EDSCALE);
	gn->add_child(pb);

	gn->set_slot(0, true, 0, Color(0, 1, 0), true, 0, Color(0, 1, 0));

	_graph_edit->add_child(gn);
}

void ProceduralAnimationEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_frame_button_pressed"), &ProceduralAnimationEditor::add_frame_button_pressed);
}

ProceduralAnimationEditor::ProceduralAnimationEditor(EditorNode *p_editor) {
	set_h_size_flags(SIZE_EXPAND_FILL);

	//top bar
	HBoxContainer *hbc = memnew(HBoxContainer);
	add_child(hbc);

	MenuButton *categtnb = memnew(MenuButton);
	categtnb->set_text("Category");

	PopupMenu *cpm = categtnb->get_popup();
	cpm->add_item("Add");
	cpm->add_item("Delete");
	cpm->add_item("Rename");

	hbc->add_child(categtnb);

	OptionButton *cob = memnew(OptionButton);
	cob->set_h_size_flags(SIZE_EXPAND_FILL);
	hbc->add_child(cob);

	MenuButton *animtn = memnew(MenuButton);
	animtn->set_text("Animation");

	PopupMenu *apm = animtn->get_popup();
	apm->add_item("Add");
	apm->add_item("Delete");
	apm->add_item("Rename");

	hbc->add_child(animtn);

	OptionButton *cab = memnew(OptionButton);
	cab->set_h_size_flags(SIZE_EXPAND_FILL);
	hbc->add_child(cab);

	Button *aafb = memnew(Button);
	aafb->set_text("add frame");
	aafb->connect("pressed", this, "add_frame_button_pressed");
	hbc->add_child(aafb);

	Button *pinb = memnew(Button);
	pinb->set_text("Pin");
	hbc->add_child(pinb);

	//bottom
	_graph_edit = memnew(GraphEdit);
	_graph_edit->set_h_size_flags(SIZE_EXPAND_FILL);
	_graph_edit->set_v_size_flags(SIZE_EXPAND_FILL);
	_graph_edit->set_custom_minimum_size(Size2(0, 200) * EDSCALE);
	add_child(_graph_edit);

	_start_node = memnew(GraphNode);
	_start_node->set_title("Start");
	_start_node->set_show_close_button(false);
	_graph_edit->add_child(_start_node);

	Label *slb = memnew(Label);
	slb->set_text("Frame");
	_start_node->add_child(slb);

	_start_node->set_slot(0, false, 0, Color(0, 1, 0), true, 0, Color(0, 1, 0));

	Node *popups = memnew(Node);
	add_child(popups);

	//name popup
	_name_popuop = memnew(ConfirmationDialog);
	popups->add_child(_name_popuop);

	VBoxContainer *name_popup_container = memnew(VBoxContainer);
	_name_popuop->add_child(name_popup_container);

	_name_pupup_label = memnew(Label);
	_name_pupup_label->set_text("Enter name");
	name_popup_container->add_child(_name_pupup_label);

	_name_popup_line_edit = memnew(LineEdit);
	name_popup_container->add_child(_name_popup_line_edit);
}
ProceduralAnimationEditor::~ProceduralAnimationEditor() {
}

void ProceduralAnimationEditorPlugin::edit(Object *p_object) {
	if (Object::cast_to<ProceduralAnimation>(p_object)) {
		animation_editor->edit(Object::cast_to<ProceduralAnimation>(p_object));
		animation_editor_button->show();
	} else
		animation_editor_button->hide();
}

bool ProceduralAnimationEditorPlugin::handles(Object *p_object) const {

	bool handles = p_object->is_class("ProceduralAnimation");

	if (handles) {
		animation_editor_button->show();
	} else {
		animation_editor_button->hide();
	}

	return handles;
}

void ProceduralAnimationEditorPlugin::make_visible(bool p_visible) {
}

ProceduralAnimationEditorPlugin::ProceduralAnimationEditorPlugin(EditorNode *p_node) {

	editor = p_node;

	animation_editor = memnew(ProceduralAnimationEditor(editor));

	animation_editor_button = add_control_to_bottom_panel(animation_editor, "Procedural Animations");

	animation_editor->hide();
}

ProceduralAnimationEditorPlugin::~ProceduralAnimationEditorPlugin() {
}
