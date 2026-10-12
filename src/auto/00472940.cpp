#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SetSlotValue, 0x00472350, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, FindTableEntry, 0x004728E0, (void* self, void* edx, int a2));

static int __fastcall S_00472350_ProcessTableEntry(
    S_00472350* self, void* /*edx*/, int entry)
{
    (void)CSF_VTBL(0x00472350);

    const int slot = FindTableEntry(self, nullptr, entry);
    if (slot == -1)
        return slot;

    return static_cast<int>(
        SetSlotValue(self, nullptr, static_cast<unsigned int>(slot)));
}

HOOK(0x00472940, S_00472350_ProcessTableEntry, "auto");
