#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GlobalHud_AttachWidget_6C8420, 0x006C8420, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, HudWidget_Refresh_4360A0, 0x004360A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (void* self, void* edx));

// Refreshes the HUD debug overlay: attaches the sub-widget at this+8 to the
// global HUD object, refreshes the child at this+4, then commits the global
// HUD object and returns that commit's result (tail call in the original).
static char __fastcall Hud_UpdateDebugOverlay(void* self, void* /*edx*/)
{
    // Global block pointer; the HUD object lives at +4 of it.
    void* globalBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_6C5760()));
    C_0085e7b8* hudObject = CSF_AT(C_0085e7b8*, globalBlock, 0x04);

    GlobalHud_AttachWidget_6C8420(hudObject, nullptr, reinterpret_cast<uint8_t*>(self) + 0x08);

    void* overlayChild = CSF_AT(void*, self, 0x04);
    HudWidget_Refresh_4360A0(overlayChild, nullptr);

    return GlobalHud_Commit_6C7C30(hudObject, nullptr);
}

HOOK(0x00436110, Hud_UpdateDebugOverlay, "auto");
