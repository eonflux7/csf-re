#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, ReleaseEntryTable, 0x0069B280, (S_004713e0* self, void*));

// Resets the four entry-table sub-objects embedded in C_0085eac0 (at +0x08, +0x4C, +0x90, +0xD4)
// and clears the state byte at +0x118.
static int __fastcall C_0085eac0_ResetEntryTables(C_0085eac0* self, void* /*edx*/) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    S_004713e0* tableA = reinterpret_cast<S_004713e0*>(base + 0x08);
    S_004713e0* tableB = reinterpret_cast<S_004713e0*>(base + 0x4C);
    S_004713e0* tableC = reinterpret_cast<S_004713e0*>(base + 0x90);
    S_004713e0* tableD = reinterpret_cast<S_004713e0*>(base + 0xD4);

    SceneLoader_NoOp();
    SceneLoader_NoOp();
    SceneLoader_NoOp();
    SceneLoader_NoOp();

    ReleaseEntryTable(tableA, nullptr);
    ReleaseEntryTable(tableB, nullptr);
    ReleaseEntryTable(tableC, nullptr);
    int result = ReleaseEntryTable(tableD, nullptr);

    self->field_0118 = 0;
    return result;
}

HOOK(0x0069DE90, C_0085eac0_ResetEntryTables, "auto");
