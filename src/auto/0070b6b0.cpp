#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_AttachWidget, 0x006C8420, (C_0085e7b8* self, void* edx, C_0085e7b8* widget));
CSF_FUNC(void, __fastcall, Water_UpdateSubsystem_70B340, 0x0070B340, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void* edx));

// Attaches the widget at this+8 to the global HUD object, updates the water
// subsystem stored at this+4, then commits the HUD object and returns that
// commit's result (tail call in the original).
static char __fastcall Water_AttachHudWidgetAndCommit(void* self, void* /*edx*/)
{
    // Global block pointer; the HUD object lives at +4 of it.
    void* globalBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_6C5760()));
    C_0085e7b8* hudObject = CSF_AT(C_0085e7b8*, globalBlock, 0x04);

    C_0085e7b8* widget = reinterpret_cast<C_0085e7b8*>(reinterpret_cast<uint8_t*>(self) + 0x08);
    C_0085e7b8_AttachWidget(hudObject, nullptr, widget);

    void* subsystem = CSF_AT(void*, self, 0x04);
    Water_UpdateSubsystem_70B340(subsystem, nullptr);

    return GlobalHud_Commit_6C7C30(hudObject, nullptr);
}

HOOK(0x0070B6B0, Water_AttachHudWidgetAndCommit, "auto");
