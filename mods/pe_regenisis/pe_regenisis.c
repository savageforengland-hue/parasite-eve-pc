/* REGENISIS integration: safe field-only chapter entry and trace hooks.
 * This is NOT the full campaign event interpreter. */
#include <stdio.h>
#include "pe_game.h"
#include "pe_mod_host.h"
#include "pe_mod_menu.h"
#include "pe1/game_state.h"
#include "pe1/scene_dispatch.h"
#include "pe1/script_queue.h"

#define MOD_ID "pe_regenisis"
#define PE_STORY_VAR 74
/* M182 entry: the original engine's field barcode encoding. */
#define REGENISIS_ENTRY_TOKEN 0xA80280C8u
#define REGENISIS_STORY 596u

static int s_trace_scenes = 1;
static int s_pending_entry;
static unsigned s_scene_count;

static int on_scene_start(void *ctx, void *user) {
    (void)ctx; (void)user;
    ++s_scene_count;
    if (s_trace_scenes)
        pe_mod_log(MOD_ID, "scene #%u room=%d token=%08X",
                   s_scene_count, pe_room_number(), (unsigned)g_SceneDispatchToken);
    return 0;
}

static int on_frame(void *ctx, void *user) {
    unsigned token;
    int destination, *args[1];
    (void)ctx; (void)user;
    if (!s_pending_entry) return 0;
    if (!pe_game_loaded()) {
        s_pending_entry = 0;
        pe_toast("REGENISIS: load a game first");
        return 0;
    }
    token = (unsigned)g_SceneDispatchToken;
    if (!token || token == 0xA9400048u || token == 0xAA108448u ||
        token == 0xA8000048u || token == 0xA80830C8u) return 0;
    if ((unsigned)g_SceneDispatchCur != token || !pe_free_field() ||
        g_GameState.scene_audio.tracks.movie_active) return 0;
    s_pending_entry = 0;
    destination = (int)REGENISIS_ENTRY_TOKEN;
    args[0] = &destination;
    g_ScriptVariableBank[PE_STORY_VAR] = REGENISIS_STORY;
    (void)Script_EncodeBarcode(args);
    pe_mod_log(MOD_ID, "chapter entry requested token=%08X story=%u",
               REGENISIS_ENTRY_TOKEN, REGENISIS_STORY);
    pe_toast("REGENISIS: entered M182 (events pending)");
    return 0;
}

static int on_new_game(void *ctx, void *user) {
    (void)ctx; (void)user;
    s_pending_entry = 0;
    return 0;
}

static void request_entry(void) {
    if (!pe_game_loaded()) {
        pe_toast("REGENISIS: load a game first");
        return;
    }
    s_pending_entry = 1;
    pe_toast("REGENISIS: enter when the field is free");
}

static void show_status(void) {
    pe_mod_log(MOD_ID, "M182 entry available; campaign events not yet implemented");
    pe_toast("REGENISIS: campaign scripting pending");
}

static int regenisis_init(void) {
    if (pe_hook_add(PE_HOOK_SCENE_START, on_scene_start, NULL,
                    PE_PRIORITY_NORMAL, MOD_ID) < 0) return -1;
    if (pe_hook_add(PE_HOOK_FRAME, on_frame, NULL,
                    PE_PRIORITY_LATE, MOD_ID) < 0) return -1;
    if (pe_hook_add(PE_HOOK_NEW_GAME, on_new_game, NULL,
                    PE_PRIORITY_NORMAL, MOD_ID) < 0) return -1;
    pe_menu_begin("REGENISIS (development)");
    pe_menu_register_toggle(0, "Trace scenes", "Log scene transitions.",
                            &s_trace_scenes, NULL);
    pe_menu_register_action(0, "Enter Forgotten Line",
                            "Experimental transfer to M182 from a loaded field game.",
                            request_entry);
    pe_menu_register_action(0, "Integration status",
                            "Shows incomplete campaign integration status.", show_status);
    pe_menu_begin(0);
    return 0;
}

const PeMod pe_mod_pe_regenisis = {
    MOD_ID, "REGENISIS (development)", "0.2.0", NULL, regenisis_init, NULL
};
