#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_AttachWidget, 0x006C8420, (C_0085e7b8* self, void* , C_0085e7b8* widget));
CSF_FUNC(char, __fastcall, GfxSkin_CheckObjectEligibility, 0x006B3870, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));

// Attaches the sub-widget at this+8 to the global HUD object, runs the
// eligibility check on the object at this+4, then commits the global HUD
// object and returns that commit's result (tail call in the original).
static char __fastcall GfxObject_AttachAndCommitHudWidget(void* self, void* /*edx*/)
{
    // Global block pointer; the HUD object lives at +4 of it.
    void* globalBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_6C5760()));
    C_0085e7b8* hudObject = CSF_AT(C_0085e7b8*, globalBlock, 0x04);

    C_0085e7b8_AttachWidget(hudObject, nullptr, reinterpret_cast<C_0085e7b8*>(reinterpret_cast<uint8_t*>(self) + 0x08));

    void* skinObject = CSF_AT(void*, self, 0x04);
    GfxSkin_CheckObjectEligibility(skinObject, nullptr);

    return GlobalHud_Commit_6C7C30(hudObject, nullptr);
}

HOOK(0x006B3F80, GfxObject_AttachAndCommitHudWidget, "auto");
