#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, S_006c5cd0_PreparePrimaryState, 0x006C5CD0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_006c5cd0_PrepareSecondaryState, 0x006C5EF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_006c5cd0_PrepareFinalState, 0x006C7990, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_CompleteEntrySetup, 0x006DA3C0, (void* self, void* edx));

static char __fastcall S_006c5cd0_ResetAndCommitState(
    S_006c5cd0* self, void* /*edx*/)
{
    // Packet-listed reference; the disassembly uses this address as a callee,
    // not as a vtable store.
    (void)CSF_VTBL(0x006C5CD0);

    const char succeeded =
        S_006c5cd0_PreparePrimaryState(self, nullptr) &&
        S_006c5cd0_PrepareSecondaryState(self, nullptr) &&
        S_006c5cd0_PrepareFinalState(self, nullptr);

    void* commitObject = reinterpret_cast<void*>(self->field_0004);
    self->field_002c = 0;
    self->field_0038 = 0;
    self->field_0048 = 0;
    self->field_0058 = 0;
    self->field_0068 = 0;
    self->field_0078 = 0;
    self->field_0088 = 0;
    self->field_0098 = 0;
    self->field_00a8 = 0;
    self->field_00b8 = 0;
    self->field_00c8 = 0;
    self->field_00d8 = 0;
    self->field_00e8 = 0;

    GlobalHud_Commit_6C7C30(commitObject, nullptr);

    const int globalManager = GetGlobalInt();
    const int clumpManager = ResolveClumpManager(globalManager, nullptr);
    const int entry = ResolveEntryLookupValue(
        reinterpret_cast<void*>(clumpManager), nullptr);
    GfxSkin_CompleteEntrySetup(reinterpret_cast<void*>(entry), nullptr);

    self->field_00f0 = 0;
    return succeeded;
}

HOOK(0x006C7A20, S_006c5cd0_ResetAndCommitState, "auto");
