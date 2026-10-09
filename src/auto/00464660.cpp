#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, sub_4728E0, 0x004728E0, (void* self, void* edx, int a2));

static void* AsPointer(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

// __thiscall(self, outputRecord, unusedArg, argRecord) ret 12
// Picks an entry from the scene/owner table using the record's index bits,
// calls it with argRecord[1], and stores the integer result as a float
// into outputRecord->field_4.
static int __fastcall LookupTableEntryAndStoreFloat(void* self, void* /*edx*/, void* outputRecord, int /*unusedArg*/, int* argRecord)
{
    uint32_t flagsWord = CSF_AT(uint32_t, AsPointer(static_cast<uint32_t>(*argRecord)), 8);

    uint32_t ownerPtr = CSF_AT(uint32_t, self, 0x20);
    if ((flagsWord & 0x800000u) != 0)
    {
        uint32_t sceneRoot = CSF_AT(uint32_t, g_pCurrentScene, 0xBC);
        ownerPtr = CSF_AT(uint32_t, AsPointer(sceneRoot), 0x38);
        flagsWord &= ~0x800000u;
    }

    uint32_t tableBase = CSF_AT(uint32_t, AsPointer(ownerPtr), 8);
    uint32_t entryObject = CSF_AT(uint32_t, AsPointer(tableBase), 12 * flagsWord + 4);

    int secondArg = argRecord[1];

    uint32_t outputTarget = CSF_AT(uint32_t, outputRecord, 4);

    int result = sub_4728E0(AsPointer(entryObject), nullptr, secondArg);

    CSF_AT(float, AsPointer(outputTarget), 4) = static_cast<float>(result);
    return result;
}

HOOK(0x00464660, LookupTableEntryAndStoreFloat, "auto");
