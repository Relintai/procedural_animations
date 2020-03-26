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

#include "procedural_animation_editor_plugin.h"

#include "editor/editor_scale.h"

void ProceduralAnimationEditor::edit(const Ref<ProceduralAnimation> &animation) {
	_animation = animation;

	_stop->hide();
	_play->hide();
	_play_from->hide();
	_play_bw->hide();
	_play_bw_from->hide();

	clear_keyframe_nodes();
}

void ProceduralAnimationEditor::load_selected_animation() {
	clear_keyframe_nodes();

	ERR_FAIL_COND(!_animation.is_valid());

	_start_node->set_offset(_animation->get_start_node_position());

	const PoolVector<String> &animation_names = _animation->get_animation_keyframe_names();

	PoolVector<int> kfind = _animation->get_keyframe_indices();

	for (int i = 0; i < kfind.size(); ++i) {
		int id = kfind[i];

		ProceduralAnimationEditorGraphNode *gn = memnew(ProceduralAnimationEditorGraphNode);
		gn->set_name(String::num(id));
		_graph_edit->add_child(gn);

		//gn->set_animation_keyframe_names(animation_names);

		gn->set_id(id);
		gn->set_offset(_animation->get_keyframe_node_position(id));
		gn->set_keyframe_name(_animation->get_keyframe_name(id));
		gn->set_next_keyframe(_animation->get_keyframe_next_keyframe_index(id));
		gn->set_in_curve(_animation->get_keyframe_in_curve(id));
		gn->set_animation_keyframe_index(_animation->get_keyframe_animation_keyframe_index(id));
		gn->connect("graphnode_changed", this, "on_keyframe_node_changed");
	}

	for (int i = 0; i < kfind.size(); ++i) {
		int id = kfind[i];

		int ni = _animation->get_keyframe_next_keyframe_index(id);

		if (ni != -1)
			_graph_edit->connect_node(String::num(id), 0, String::num(ni), 0);
	}

	int st = _animation->get_start_frame_index();

	if (st != -1)
		_graph_edit->connect_node("Start", 0, String::num(st), 0);
}

void ProceduralAnimationEditor::clear_keyframe_nodes() {
	_graph_edit->clear_connections();

	for (int i = 0; i < _graph_edit->get_child_count(); ++i) {
		Node *n = _graph_edit->get_child(i);

		ProceduralAnimationEditorGraphNode *gn = Object::cast_to<ProceduralAnimationEditorGraphNode>(n);

		if (!ObjectDB::instance_validate(gn)) {
			continue;
		}

		gn->disconnect("graphnode_changed", this, "on_keyframe_node_changed");
		gn->queue_delete();
	}
}

void ProceduralAnimationEditor::on_keyframe_node_changed(Node *node) {
	ProceduralAnimationEditorGraphNode *gn = Object::cast_to<ProceduralAnimationEditorGraphNode>(node);

	ERR_FAIL_COND(!ObjectDB::instance_validate(gn));
	ERR_FAIL_COND(!_animation.is_valid());

	int id = gn->get_id();

	_animation->set_keyframe_animation_keyframe_index(id, gn->get_animation_keyframe_index());
	_animation->set_keyframe_in_curve(id, gn->get_in_curve());
	_animation->set_keyframe_name(id, gn->get_keyframe_name());
	_animation->set_keyframe_next_keyframe_index(id, gn->get_next_keyframe());
	_animation->set_keyframe_node_position(id, gn->get_offset());
}

void ProceduralAnimationEditor::on_delete_popup_confirmed() {
	switch (_delete_popup_action) {
		case DELETE_POPUP_KEYFRAME:
			break;
	}
}

void ProceduralAnimationEditor::on_connection_request(const String &from, const int from_slot, const String &to, const int to_slot) {
	Node *f = _graph_edit->get_node_or_null(from);

	ProceduralAnimationEditorGraphNode *gn = Object::cast_to<ProceduralAnimationEditorGraphNode>(f);

	if (gn != NULL) {
		int ni = _animation->get_keyframe_next_keyframe_index(gn->get_id());

		if (ni != -1) {
			_graph_edit->disconnect_node(from, from_slot, String::num(ni), 0);
		}

		_animation->set_keyframe_next_keyframe_index(gn->get_id(), to.to_int());
	} else {
		GraphNode *g = Object::cast_to<GraphNode>(f);

		if (g != NULL) {
			int st = _animation->get_start_frame_index();

			if (st != -1) {
				_graph_edit->disconnect_node("Start", 0, String::num(st), 0);
			}

			_animation->set_start_frame_index(to.to_int());
		}
	}

	_graph_edit->connect_node(from, from_slot, to, to_slot);
}
void ProceduralAnimationEditor::on_disconnection_request(const String &from, const int from_slot, const String &to, const int to_slot) {
	Node *f = _graph_edit->get_node_or_null(from);

	ProceduralAnimationEditorGraphNode *gn = Object::cast_to<ProceduralAnimationEditorGraphNode>(f);

	if (gn != NULL) {
		_animation->set_keyframe_next_keyframe_index(gn->get_id(), -1);
	} else {
		GraphNode *g = Object::cast_to<GraphNode>(f);

		if (g != NULL) {
			_animation->set_start_frame_index(-1);
		}
	}

	_graph_edit->disconnect_node(from, from_slot, to, to_slot);
}

void ProceduralAnimationEditor::add_frame_button_pressed() {
	int id = _animation->add_keyframe();

	ProceduralAnimationEditorGraphNode *gn = memnew(ProceduralAnimationEditorGraphNode);
	gn->set_id(id);
	_graph_edit->add_child(gn);
}

void ProceduralAnimationEditor::_notification(int p_what) {

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

void ProceduralAnimationEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_delete_popup_confirmed"), &ProceduralAnimationEditor::on_delete_popup_confirmed);

	ClassDB::bind_method(D_METHOD("add_frame_button_pressed"), &ProceduralAnimationEditor::add_frame_button_pressed);

	ClassDB::bind_method(D_METHOD("on_keyframe_node_changed", "node"), &ProceduralAnimationEditor::on_keyframe_node_changed);

	ClassDB::bind_method(D_METHOD("on_connection_request", "from", "from_slot", "to", "to_slot"), &ProceduralAnimationEditor::on_connection_request);
	ClassDB::bind_method(D_METHOD("on_disconnection_request", "from", "from_slot", "to", "to_slot"), &ProceduralAnimationEditor::on_disconnection_request);
}

ProceduralAnimationEditor::ProceduralAnimationEditor() {
}

ProceduralAnimationEditor::ProceduralAnimationEditor(EditorNode *p_editor) {
	set_h_size_flags(SIZE_EXPAND_FILL);

	//top bar
	HBoxContainer *hbc = memnew(HBoxContainer);
	add_child(hbc);

	_animation_option_button = memnew(OptionButton);
	_animation_option_button->set_h_size_flags(SIZE_EXPAND_FILL);
	_animation_option_button->connect("item_selected", this, "on_animation_option_button_pressed");
	hbc->add_child(_animation_option_button);

	Button *aafb = memnew(Button);
	aafb->set_text("add frame");
	aafb->connect("pressed", this, "add_frame_button_pressed");
	hbc->add_child(aafb);

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

	//bottom
	_graph_edit = memnew(GraphEdit);
	_graph_edit->set_right_disconnects(true);
	_graph_edit->set_h_size_flags(SIZE_EXPAND_FILL);
	_graph_edit->set_v_size_flags(SIZE_EXPAND_FILL);
	_graph_edit->set_custom_minimum_size(Size2(0, 200) * EDSCALE);
	_graph_edit->connect("connection_request", this, "on_connection_request");
	_graph_edit->connect("disconnection_request", this, "on_disconnection_request");
	add_child(_graph_edit);

	_start_node = memnew(GraphNode);
	_start_node->set_name("Start");
	_start_node->set_title("Start");
	_start_node->set_show_close_button(false);
	_graph_edit->add_child(_start_node);

	Label *slb = memnew(Label);
	slb->set_text("Frame");
	_start_node->add_child(slb);

	_start_node->set_slot(0, false, 0, Color(0, 1, 0), true, 0, Color(0, 1, 0));

	Control *popups = memnew(Control);
	add_child(popups);

	//delete confirm popup
	_delete_popuop = memnew(ConfirmationDialog);
	_delete_popuop->connect("confirmed", this, "on_delete_popup_confirmed");
	popups->add_child(_delete_popuop);

	Label *dellabel = memnew(Label);
	dellabel->set_text("Delete?");
	_delete_popuop->add_child(dellabel);

	//name popup
	_name_popuop = memnew(ConfirmationDialog);
	_name_popuop->connect("confirmed", this, "on_name_popup_confirmed");
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

int ProceduralAnimationEditorGraphNode::get_id() const {
	return _id;
}
void ProceduralAnimationEditorGraphNode::set_id(const int id) {
	_id = id;
	emit_signal("graphnode_changed", this);
}

String ProceduralAnimationEditorGraphNode::get_keyframe_name() const {
	return _name->get_text();
}
void ProceduralAnimationEditorGraphNode::set_keyframe_name(const String &value) {
	_name->set_text(value);
	emit_signal("graphnode_changed", this);
}
void ProceduralAnimationEditorGraphNode::on_keyframe_name_modified(const String &value) {
	emit_signal("graphnode_changed", this);
}

int ProceduralAnimationEditorGraphNode::get_animation_keyframe_index() const {
	return _animation_keyframe_index;
}
void ProceduralAnimationEditorGraphNode::set_animation_keyframe_index(const int value) {
	if (_animation_keyframe_index == value)
		return;

	if (_animation_keyframe_spinbox->get_line_edit()->get_text().to_int() != value) {
		_animation_keyframe_spinbox->get_line_edit()->set_text(String::num(value));
	}

	_animation_keyframe_index = value;
	//_animation_keyframe_index_option_button->select(value);
	emit_signal("graphnode_changed", this);
}

int ProceduralAnimationEditorGraphNode::get_next_keyframe() const {
	return _next_keyframe;
}
void ProceduralAnimationEditorGraphNode::set_next_keyframe(const int value) {
	_next_keyframe = value;
	emit_signal("graphnode_changed", this);
}

Ref<Curve> ProceduralAnimationEditorGraphNode::get_in_curve() const {
	return _in_curve;
}
void ProceduralAnimationEditorGraphNode::set_in_curve(const Ref<Curve> &value) {
	if (_in_curve.is_valid()) {
		_in_curve->disconnect(CoreStringNames::get_singleton()->changed, this, "changed");
	}

	_in_curve = value;
	_curve_editor->set_curve(value);
	_in_curve->connect(CoreStringNames::get_singleton()->changed, this, "changed");
	emit_signal("graphnode_changed", this);
}

void ProceduralAnimationEditorGraphNode::set_animation_keyframe_names(const PoolVector<String> &names) {
	//_animation_keyframe_index_option_button->clear();

	//for (int i = 0; i < names.size(); ++i) {
	//	const String &s = names[i];

	//	_animation_keyframe_index_option_button->add_item(s, s.get_slicec(' ', 0).to_int());
	//}
}

void ProceduralAnimationEditorGraphNode::on_animation_keyframe_spinbox_value_changed(const String &value) {
	set_animation_keyframe_index(value.to_int());
}

void ProceduralAnimationEditorGraphNode::changed() {
	emit_signal("graphnode_changed", this);
}

ProceduralAnimationEditorGraphNode::ProceduralAnimationEditorGraphNode() {
	_id = 0;

	_animation_keyframe_index = 0;
	_next_keyframe = 0;

	set_title("Animation Frame");
	set_show_close_button(true);

	Label *l1 = memnew(Label);
	l1->set_text("Name");
	add_child(l1);

	_name = memnew(LineEdit);
	_name->connect("text_entered", this, "on_keyframe_name_modified");
	add_child(_name);

	Label *l2 = memnew(Label);
	l2->set_text("Keyframe");
	add_child(l2);

	_animation_keyframe_spinbox = memnew(SpinBox);
	_animation_keyframe_spinbox->set_max(999999999);
	_animation_keyframe_spinbox->set_h_size_flags(SIZE_EXPAND_FILL);
	_animation_keyframe_spinbox->get_line_edit()->connect("text_changed", this, "on_animation_keyframe_spinbox_value_changed");
	add_child(_animation_keyframe_spinbox);

	//HBoxContainer *kfc = memnew(HBoxContainer);
	//add_child(kfc);

	//_animation_keyframe_index_option_button = memnew(OptionButton);
	//_animation_keyframe_index_option_button->set_h_size_flags(SIZE_EXPAND_FILL);
	//_animation_keyframe_index_option_button->connect("item_selected", this, "set_animation_keyframe_index");
	//kfc->add_child(_animation_keyframe_index_option_button);

	//Button *kb = memnew(Button);
	//kb->set_text("E");
	//kfc->add_child(kb);

	Label *l3 = memnew(Label);
	l3->set_text("In Curve");
	add_child(l3);

	_curve_editor = memnew(CurveEditor);
	_curve_editor->set_custom_minimum_size(Size2(0, 69) * EDSCALE);
	add_child(_curve_editor);

	set_slot(0, true, 0, Color(0, 1, 0), true, 0, Color(0, 1, 0));
}

ProceduralAnimationEditorGraphNode::~ProceduralAnimationEditorGraphNode() {
	_in_curve.unref();
}

void ProceduralAnimationEditorGraphNode::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			connect("offset_changed", this, "changed");
			break;
	}
}

void ProceduralAnimationEditorGraphNode::_bind_methods() {
	ADD_SIGNAL(MethodInfo("graphnode_changed", PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node")));

	ClassDB::bind_method(D_METHOD("on_keyframe_name_modified", "name"), &ProceduralAnimationEditorGraphNode::on_keyframe_name_modified);
	ClassDB::bind_method(D_METHOD("changed"), &ProceduralAnimationEditorGraphNode::changed);
	ClassDB::bind_method(D_METHOD("on_animation_keyframe_spinbox_value_changed", "value"), &ProceduralAnimationEditorGraphNode::on_animation_keyframe_spinbox_value_changed);
}

void ProceduralAnimationEditorPlugin::edit(Object *p_object) {
	if (Object::cast_to<ProceduralAnimation>(p_object)) {
		animation_editor->edit(Object::cast_to<ProceduralAnimation>(p_object));
		animation_editor_button->show();
	} else {
		animation_editor_button->set_pressed(false);
		animation_editor_button->hide();
	}
}

bool ProceduralAnimationEditorPlugin::handles(Object *p_object) const {
	bool player = p_object->is_class("ProceduralAnimationPlayer");
	bool handles = p_object->is_class("ProceduralAnimation") || player;

	if (handles) {
		animation_editor_button->show();

		if (player)
			animation_editor_button->set_pressed(true);
	} else {
		animation_editor_button->set_pressed(false);

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
