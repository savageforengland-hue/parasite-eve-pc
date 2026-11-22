/* REGENISIS native integration scaffold. No campaign takeover yet. */
#include "pe_mod_host.h"
#include "pe_mod_menu.h"

#define MOD_ID "pe_regenisis"
static int s_trace_scenes = 1;
static unsigned s_scene_count;

static int on_scene_start(void *ctx, void *user) {
    (void)ctx; (void)user;
    ++s_scene_count;
    if (s_trace_scenes)
        pe_mod_log(MOD_ID, "scene start #%u (campaign hooks pending)", s_scene_count);
    return 0;
}

static void show_status(void) {
    pe_mod_log(MOD_ID, "REGENISIS scaffold active; campaign routing NOT implemented");
    pe_toast("REGENISIS: engine integration pending");
}

static int regenisis_init(void) {
    if (pe_hook_add(PE_HOOK_SCENE_START, on_scene_start, NULL,
                    PE_PRIORITY_NORMAL, MOD_ID) < 0)
        return -1;
    pe_menu_begin("REGENISIS (development)");
    pe_menu_register_toggle(0, "Trace scenes", "Log scene starts for campaign integration.",
                            &s_trace_scenes, NULL);
    pe_menu_register_action(0, "Integration status", "Show current implementation status.",
                            show_status);
    pe_menu_begin(0);
    return 0;
}

const PeMod pe_mod_pe_regenisis = {
    MOD_ID, "REGENISIS (development)", "0.1.0", NULL, regenisis_init, NULL
};
