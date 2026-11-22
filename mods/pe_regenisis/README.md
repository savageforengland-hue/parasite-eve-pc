# REGENISIS Windows native integration (work in progress)

This branch now includes a **native C mod scaffold** using the existing PE mod host.
It logs scene transitions and adds a development F1 panel. It does **not**
override the original campaign or load REGENISIS JSON yet.

Build with the existing mod-enabled configuration and add:
`-DPE_EXTRA_MODS=pe_regenisis`

The Linux release's missing `regenisis.patch` must still be recovered or
its functionality independently implemented. The campaign JSON uses entry
`m0182i`, rooms 182-190, and event triggers for enter/explore/victory.
Do not advertise this as a playable expansion until runtime routing,
dialogue, encounters, and game-media tests pass.
