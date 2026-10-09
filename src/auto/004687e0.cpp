#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneFindStateRecord, 0x004686F0, (void* self, void* edx, int a2, int a3));

// Returns true when the pair link cannot be followed: the handle does not resolve,
// no state record is found, or the record's dword at +0x18 is zero.
static bool __fastcall C_00833b88_IsPairLinkMissing(C_00833b88* self, void* /*edx*/, int a2, int a3) {
    int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
    if (resolvedHandle == 0) {
        return true;
    }

    int stateRecord = SceneFindStateRecord(self, nullptr, resolvedHandle, a3);
    if (stateRecord == 0) {
        return true;
    }

    auto* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(stateRecord));
    return CSF_AT(int, record, 0x18) == 0;
}

HOOK(0x004687E0, C_00833b88_IsPairLinkMissing, "auto");
