#include "register_types.h"

#include "procedural_animation.h"
#include "procedural_animation_player.h"

#include "procedural_animation_editor_plugin.h"

void register_procedural_animations_types() {
	ClassDB::register_class<ProceduralAnimation>();
	ClassDB::register_class<ProceduralAnimationPlayer>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<ProceduralAnimationEditorPlugin>();
#endif
}

void unregister_procedural_animations_types() {
}
