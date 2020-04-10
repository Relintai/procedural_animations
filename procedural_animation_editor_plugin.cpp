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

#include "editor/editor_properties.h"

#include "editor/editor_scale.h"

#include "core/object.h"
#include "scene/animation/animation_player.h"

// S  --------        ProceduralAnimationEditor        --------

void ProceduralAnimationEditor::edit(const Ref<ProceduralAnimation> &animation) {
	_animation = animation;

	clear_keyframe_nodes();

	if (animation.is_valid())
		load_animation();

	_animation_target_animation_property->update_property();
	_animation_fps_spinbox->set_value(_animation->get_animation_fps());
	_loop_checkbox->set_pressed(_animation->has_loop());
}

void ProceduralAnimationEditor::load_animation() {
	clear_keyframe_nodes();

	ERR_FAIL_COND(!_animation.is_valid());

	_start_node->set_offset(_animation->get_start_node_position());

	const PoolVector<String> &animation_names = _animation->get_animation_keyframe_names();

	PoolVector<int> kfind = _animation->get_keyframe_indices();

	for (int i = 0; i < kfind.size(); ++i) {
		int id = kfind[i];

		ProceduralAnimationEditorGraphNode *gn = memnew(ProceduralAnimationEditorGraphNode(this));
		_graph_edit->add_child(gn);
		gn->set_name(String::num(id));
		gn->set_id(id);
		gn->set_animation(_animation);
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

		if (n == _start_node) {
			continue;
		}

		ProceduralAnimationEditorGraphNode *gn = Object::cast_to<ProceduralAnimationEditorGraphNode>(n);

		if (!gn) {
			continue;
		}

		gn->set_name("d" + gn->get_name());
		gn->queue_delete();
	}
}

void ProceduralAnimationEditor::on_delete_popup_confirmed() {
	switch (_delete_popup_action) {
		case DELETE_POPUP_KEYFRAME:
			break;
	}
}

void ProceduralAnimationEditor::on_connection_request(const String &from, const int from_slot, const String &to, const int to_slot) {
	List<GraphEdit::Connection> conns;
	_graph_edit->get_connection_list(&conns);

	//Lets make sure that there are no loops in the keyframe graph. Easiest way to check in this case,
	//is to just make sure that nothing is connected to the current node.
	//Note, that loops can still be created, but those will not be conected to the start node,
	//so those are not a problem.
	for (List<GraphEdit::Connection>::Element *E = conns.front(); E; E = E->next()) {
		GraphEdit::Connection c = E->get();

		if (c.to == to) {
			return;
		}
	}

	//no loops found

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

void ProceduralAnimationEditor::on_delete_nodes_request() {

	List<StringName> to_erase;

	for (int i = 0; i < _graph_edit->get_child_count(); i++) {
		GraphNode *gn = Object::cast_to<GraphNode>(_graph_edit->get_child(i));
		if (gn) {
			if (gn->is_selected() && gn->is_close_button_visible()) {
				to_erase.push_back(gn->get_name());
			}
		}
	}

	if (to_erase.empty())
		return;

	//undo_redo->create_action(TTR("Delete Node(s)"));

	for (List<StringName>::Element *F = to_erase.front(); F; F = F->next()) {
		_delete_request(F->get());
	}

	//undo_redo->commit_action();
}

void ProceduralAnimationEditor::add_frame_button_pressed() {
	int id = _animation->add_keyframe();

	ProceduralAnimationEditorGraphNode *gn = memnew(ProceduralAnimationEditorGraphNode(this));
	_graph_edit->add_child(gn);
	gn->set_name(String::num(id));
	gn->set_id(id);
	gn->set_animation(_animation);
}

Ref<Animation> ProceduralAnimationEditor::get_animation_target_animation() {
	if (!_animation.is_valid())
		return Ref<Animation>();

	return _animation->get_animation();
}
void ProceduralAnimationEditor::set_animation_target_animation(const Ref<Animation> &animation) {
	if (!_animation.is_valid())
		return;

	_animation->set_animation(animation);
}

void ProceduralAnimationEditor::on_animation_fps_changed(const float value) {
	if (!_animation.is_valid())
		return;

	if (_animation->get_animation_fps() == value)
		return;

	_animation->set_animation_fps(value);
}

void ProceduralAnimationEditor::on_loop_checkbox_toggled(const bool value) {
	if (!_animation.is_valid())
		return;

	if (_animation->has_loop() == value)
		return;

	_animation->set_loop(value);
}

void ProceduralAnimationEditor::_delete_request(const StringName &name) {
	if (!_animation.is_valid())
		return;

	Node *n = _graph_edit->get_node_or_null(NodePath(name));

	ProceduralAnimationEditorGraphNode *g = Object::cast_to<ProceduralAnimationEditorGraphNode>(n);

	if (g == NULL)
		return;

	List<GraphEdit::Connection> conns;
	_graph_edit->get_connection_list(&conns);

	for (List<GraphEdit::Connection>::Element *E = conns.front(); E; E = E->next()) {
		GraphEdit::Connection c = E->get();

		if (c.to == name) {
			if (c.from == "Start") {
				_animation->set_start_frame_index(-1);
			} else {
				Node *fn = _graph_edit->get_node_or_null(NodePath(c.from));

				ProceduralAnimationEditorGraphNode *fg = Object::cast_to<ProceduralAnimationEditorGraphNode>(fn);

				if (fg != NULL) {
					_animation->set_keyframe_next_keyframe_index(fg->get_id(), -1);
				}
			}

			_graph_edit->disconnect_node(c.from, c.from_port, c.to, c.to_port);
		} else if (c.from == name) {
			_graph_edit->disconnect_node(c.from, c.from_port, c.to, c.to_port);
		}
	}

	_animation->remove_keyframe(g->get_id());
	g->set_name("r" + g->get_name());
	g->queue_delete();
}

void ProceduralAnimationEditor::_notification(int p_what) {

	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED: {
#if VERSION_MAJOR < 4
			_pin->set_icon(get_icon("Pin", "EditorIcons"));
#else
			_pin->set_icon(get_theme_icon("Pin", "EditorIcons"));
#endif
		} break;
	}
}

void ProceduralAnimationEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_delete_popup_confirmed"), &ProceduralAnimationEditor::on_delete_popup_confirmed);

	ClassDB::bind_method(D_METHOD("add_frame_button_pressed"), &ProceduralAnimationEditor::add_frame_button_pressed);

	ClassDB::bind_method(D_METHOD("on_connection_request", "from", "from_slot", "to", "to_slot"), &ProceduralAnimationEditor::on_connection_request);
	ClassDB::bind_method(D_METHOD("on_disconnection_request", "from", "from_slot", "to", "to_slot"), &ProceduralAnimationEditor::on_disconnection_request);
	ClassDB::bind_method(D_METHOD("on_delete_nodes_request"), &ProceduralAnimationEditor::on_delete_nodes_request);

	ClassDB::bind_method(D_METHOD("get_animation_target_animation"), &ProceduralAnimationEditor::get_animation_target_animation);
	ClassDB::bind_method(D_METHOD("set_animation_target_animation", "animation"), &ProceduralAnimationEditor::set_animation_target_animation);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation_target_animation", PROPERTY_HINT_RESOURCE_TYPE, "Animation"), "set_animation_target_animation", "get_animation_target_animation");

	ClassDB::bind_method(D_METHOD("on_animation_fps_changed", "value"), &ProceduralAnimationEditor::on_animation_fps_changed);
	ClassDB::bind_method(D_METHOD("on_loop_checkbox_toggled", "value"), &ProceduralAnimationEditor::on_loop_checkbox_toggled);
}

ProceduralAnimationEditor::ProceduralAnimationEditor() {
}

ProceduralAnimationEditor::ProceduralAnimationEditor(EditorNode *p_editor) {
	set_h_size_flags(SIZE_EXPAND_FILL);

	//top bar
	HBoxContainer *hbc = memnew(HBoxContainer);
	add_child(hbc);

	_animation_target_animation_property = memnew(EditorPropertyResource);
	_animation_target_animation_property->set_margin(MARGIN_TOP, 5 * EDSCALE);
	_animation_target_animation_property->set_margin(MARGIN_BOTTOM, 5 * EDSCALE);
	_animation_target_animation_property->set_custom_minimum_size(Size2(300 * EDSCALE, 0));
	_animation_target_animation_property->set_label("Animation");
	_animation_target_animation_property->set_object_and_property(this, "animation_target_animation");
	hbc->add_child(_animation_target_animation_property);

	Label *fl = memnew(Label);
	fl->set_text("FPS");
	hbc->add_child(fl);

	_animation_fps_spinbox = memnew(SpinBox);
	_animation_fps_spinbox->set_max(999999999);
	_animation_fps_spinbox->set_step(1);

#if VERSION_MAJOR < 4
	_animation_fps_spinbox->connect("value_changed", this, "on_animation_fps_changed");
#else
	_animation_fps_spinbox->connect("value_changed", callable_mp(this, &ProceduralAnimationEditor::on_animation_fps_changed));
#endif

	hbc->add_child(_animation_fps_spinbox);

	_loop_checkbox = memnew(CheckBox);
	_loop_checkbox->set_text("Loop");

#if VERSION_MAJOR < 4
	_loop_checkbox->connect("toggled", this, "on_loop_checkbox_toggled");
#else
	_loop_checkbox->connect("toggled", callable_mp(this, &ProceduralAnimationEditor::on_loop_checkbox_toggled));
#endif

	hbc->add_child(_loop_checkbox);

	Control *spacer = memnew(Control);
	spacer->set_h_size_flags(SIZE_EXPAND_FILL);
	hbc->add_child(spacer);

	Button *aafb = memnew(Button);
	aafb->set_text("add frame");

#if VERSION_MAJOR < 4
	aafb->connect("pressed", this, "add_frame_button_pressed");
#else
	aafb->connect("pressed", callable_mp(this, &ProceduralAnimationEditor::add_frame_button_pressed));
#endif

	hbc->add_child(aafb);

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

#if VERSION_MAJOR < 4
	_graph_edit->connect("connection_request", this, "on_connection_request");
	_graph_edit->connect("disconnection_request", this, "on_disconnection_request");
	_graph_edit->connect("delete_nodes_request", this, "on_delete_nodes_request");
#else
	_graph_edit->connect("connection_request", callable_mp(this, &ProceduralAnimationEditor::on_connection_request));
	_graph_edit->connect("disconnection_request", callable_mp(this, &ProceduralAnimationEditor::on_disconnection_request));
	_graph_edit->connect("delete_nodes_request", callable_mp(this, &ProceduralAnimationEditor::on_delete_nodes_request));
#endif

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

#if VERSION_MAJOR < 4
	_delete_popuop->connect("confirmed", this, "on_delete_popup_confirmed");
#else
	_delete_popuop->connect("confirmed", callable_mp(this, &ProceduralAnimationEditor::on_delete_popup_confirmed));
#endif

	popups->add_child(_delete_popuop);

	Label *dellabel = memnew(Label);
	dellabel->set_text("Delete?");
	_delete_popuop->add_child(dellabel);

	//name popup
	_name_popuop = memnew(ConfirmationDialog);
	//_name_popuop->connect("confirmed", this, "on_name_popup_confirmed");
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

// E  --------        ProceduralAnimationEditor        --------

// S  --------        ProceduralAnimationEditorGraphNode        --------

int ProceduralAnimationEditorGraphNode::get_id() const {
	return _id;
}
void ProceduralAnimationEditorGraphNode::set_id(const int id) {
	_id = id;
}

String ProceduralAnimationEditorGraphNode::get_keyframe_name() const {
	return _name->get_text();
}
void ProceduralAnimationEditorGraphNode::set_keyframe_name(const String &value) {
	_name->set_text(value);

	if (!_animation.is_valid())
		return;

	_animation->set_keyframe_name(_id, value);

	changed();
}
void ProceduralAnimationEditorGraphNode::on_keyframe_name_modified(const String &value) {
	set_keyframe_name(value);
}

int ProceduralAnimationEditorGraphNode::get_animation_keyframe_index() const {
	return _animation_keyframe_index;
}
void ProceduralAnimationEditorGraphNode::set_animation_keyframe_index(const int value) {
	if (_animation_keyframe_index == value)
		return;

	_animation_keyframe_spinbox->set_value(value);

	_animation_keyframe_index = value;

	if (!_animation.is_valid())
		return;

	_animation->set_keyframe_animation_keyframe_index(_id, value);

	changed();
}

int ProceduralAnimationEditorGraphNode::get_next_keyframe() const {
	return _next_keyframe;
}
void ProceduralAnimationEditorGraphNode::set_next_keyframe(const int value) {
	_next_keyframe = value;

	if (!_animation.is_valid())
		return;

	_animation->set_keyframe_next_keyframe_index(_id, value);

	changed();
}

float ProceduralAnimationEditorGraphNode::get_transition() const {
	return _transition;
}
void ProceduralAnimationEditorGraphNode::set_transition(const float value) {
	_transition = value;

	if (!_animation.is_valid())
		return;

	_animation->set_keyframe_transition(_id, value);

	changed();
}

float ProceduralAnimationEditorGraphNode::get_time() const {
	return _time;
}
void ProceduralAnimationEditorGraphNode::set_time(const float value) {
	if (Math::is_equal_approx(value, _time))
		return;

	_time = value;
	_time_spinbox->set_value(value);

	if (!_animation.is_valid())
		return;

	_animation->set_keyframe_time(_id, value);

	changed();
}

Ref<ProceduralAnimation> ProceduralAnimationEditorGraphNode::get_animation() {
	return _animation;
}
void ProceduralAnimationEditorGraphNode::set_animation(const Ref<ProceduralAnimation> &animation) {
	_animation.unref();

	if (!animation.is_valid())
		return;

	set_offset(animation->get_keyframe_node_position(_id));
	set_keyframe_name(animation->get_keyframe_name(_id));
	set_next_keyframe(animation->get_keyframe_next_keyframe_index(_id));
	set_transition(animation->get_keyframe_transition(_id));
	set_animation_keyframe_index(animation->get_keyframe_animation_keyframe_index(_id));
	set_time(animation->get_keyframe_time(_id));

	_animation = animation;
}

ProceduralAnimationEditorGraphNode::ProceduralAnimationEditorGraphNode(ProceduralAnimationEditor *editor) {
	_editor = editor;

	_id = 0;

	_animation_keyframe_index = 0;
	_next_keyframe = -1;
	_time = 1;
	_transition = 1.0;

	set_title("Animation Frame");
	set_show_close_button(true);

	Label *l1 = memnew(Label);
	l1->set_text("Name");
	add_child(l1);

	_name = memnew(LineEdit);

#if VERSION_MAJOR < 4
	_name->connect("text_entered", this, "on_keyframe_name_modified");
#else
	_name->connect("text_entered", callable_mp(this, &ProceduralAnimationEditorGraphNode::on_keyframe_name_modified));
#endif

	add_child(_name);

	Label *l2 = memnew(Label);
	l2->set_text("Keyframe");
	add_child(l2);

	_animation_keyframe_spinbox = memnew(SpinBox);
	_animation_keyframe_spinbox->set_max(999999999);
	_animation_keyframe_spinbox->set_h_size_flags(SIZE_EXPAND_FILL);

#if VERSION_MAJOR < 4
	_animation_keyframe_spinbox->connect("value_changed", this, "on_animation_keyframe_spinbox_value_changed");
#else
	_animation_keyframe_spinbox->connect("value_changed", callable_mp(this, &ProceduralAnimationEditorGraphNode::on_animation_keyframe_spinbox_value_changed));
#endif

	add_child(_animation_keyframe_spinbox);

	Label *lt = memnew(Label);
	lt->set_text("Time");
	add_child(lt);

	_time_spinbox = memnew(SpinBox);
	_time_spinbox->set_max(999999999);
	_time_spinbox->set_min(0);
	_time_spinbox->set_step(0.01);
	_time_spinbox->set_value(1);
	_time_spinbox->set_h_size_flags(SIZE_EXPAND_FILL);

#if VERSION_MAJOR < 4
	_time_spinbox->connect("value_changed", this, "on_time_spinbox_value_changed");
#else
	_time_spinbox->connect("value_changed", callable_mp(this, &ProceduralAnimationEditorGraphNode::on_time_spinbox_value_changed));
#endif

	add_child(_time_spinbox);

	_transition_editor = memnew(EditorPropertyEasing);
	_transition_editor->set_margin(MARGIN_TOP, 5 * EDSCALE);
	_transition_editor->set_margin(MARGIN_BOTTOM, 5 * EDSCALE);
	_transition_editor->set_label("Easing");
	_transition_editor->set_object_and_property(this, "transition");
	_transition_editor->set_h_size_flags(SIZE_EXPAND_FILL);
	add_child(_transition_editor);

	set_slot(0, true, 0, Color(0, 1, 0), true, 0, Color(0, 1, 0));
}

ProceduralAnimationEditorGraphNode::~ProceduralAnimationEditorGraphNode() {
}

void ProceduralAnimationEditorGraphNode::on_animation_keyframe_spinbox_value_changed(float value) {
	set_animation_keyframe_index(value);
}

void ProceduralAnimationEditorGraphNode::on_time_spinbox_value_changed(float value) {
	set_time(value);
}

void ProceduralAnimationEditorGraphNode::on_offset_changed() {
	if (!_animation.is_valid())
		return;

	_animation->set_keyframe_node_position(_id, get_offset());

	changed();
}

void ProceduralAnimationEditorGraphNode::changed() {
	emit_signal("graphnode_changed", this);
}

void ProceduralAnimationEditorGraphNode::on_transition_changed(const StringName &p_property, const Variant &p_value, const StringName &p_field, bool p_changing) {
	set_transition(p_value);
}

void ProceduralAnimationEditorGraphNode::on_close_request() {
	_editor->_delete_request(get_name());
}

void ProceduralAnimationEditorGraphNode::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
#if VERSION_MAJOR < 4
			connect("offset_changed", this, "on_offset_changed");
			_transition_editor->connect("property_changed", this, "on_transition_changed");
			connect("close_request", this, "on_close_request");
#else
			connect("offset_changed", callable_mp(this, &ProceduralAnimationEditorGraphNode::on_offset_changed));
			_transition_editor->connect("property_changed", callable_mp(this, &ProceduralAnimationEditorGraphNode::on_transition_changed));
			connect("close_request", callable_mp(this, &ProceduralAnimationEditorGraphNode::on_close_request));
#endif
			break;
	}
}

void ProceduralAnimationEditorGraphNode::_bind_methods() {
	ADD_SIGNAL(MethodInfo("graphnode_changed", PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node")));

	ClassDB::bind_method(D_METHOD("on_keyframe_name_modified", "name"), &ProceduralAnimationEditorGraphNode::on_keyframe_name_modified);
	ClassDB::bind_method(D_METHOD("changed"), &ProceduralAnimationEditorGraphNode::changed);
	ClassDB::bind_method(D_METHOD("on_animation_keyframe_spinbox_value_changed", "value"), &ProceduralAnimationEditorGraphNode::on_animation_keyframe_spinbox_value_changed);
	ClassDB::bind_method(D_METHOD("on_time_spinbox_value_changed", "value"), &ProceduralAnimationEditorGraphNode::on_time_spinbox_value_changed);

	ClassDB::bind_method(D_METHOD("on_offset_changed"), &ProceduralAnimationEditorGraphNode::on_offset_changed);

	ClassDB::bind_method(D_METHOD("on_close_request"), &ProceduralAnimationEditorGraphNode::on_close_request);

	ClassDB::bind_method(D_METHOD("on_transition_changed", "p_property", "p_value", "p_field", "p_changing"), &ProceduralAnimationEditorGraphNode::on_transition_changed);

	ClassDB::bind_method(D_METHOD("get_transition"), &ProceduralAnimationEditorGraphNode::get_transition);
	ClassDB::bind_method(D_METHOD("set_transition", "value"), &ProceduralAnimationEditorGraphNode::set_transition);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "transition", PROPERTY_HINT_EXP_EASING), "set_transition", "get_transition");
}

// E  --------        ProceduralAnimationEditorGraphNode        --------

// S  --------        ProceduralAnimationEditorPlugin        --------

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
	if (p_object->is_class("ProceduralAnimation")) {
		animation_editor_button->show();
		animation_editor_button->set_pressed(true);
		return true;
	}

	if (p_object->is_class("AnimationPlayer")) {
		AnimationPlayer *player = Object::cast_to<AnimationPlayer>(p_object);

		String an = player->get_assigned_animation();

		if (an != "") {
			Ref<ProceduralAnimation> anim = player->get_animation(an);

			if (anim.is_valid()) {
				animation_editor_button->show();
				//animation_editor_button->set_pressed(true);
				return true;
			}
		}
	}

	animation_editor_button->set_pressed(false);
	animation_editor_button->hide();

	return false;
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

// E  --------        ProceduralAnimationEditorPlugin        --------