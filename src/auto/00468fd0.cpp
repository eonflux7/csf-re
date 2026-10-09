#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(uint32_t*, __fastcall, sub_467AD0, 0x00467AD0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_468BB0, 0x00468BB0, (void* self, void* edx, int a2));

// Allocates a 0x84-byte entry, initializes it from a2, appends it to the
// entry list at this+0x28 (growing it when full), then runs the registration step.
static uint32_t* __fastcall MissionScript_AllocAndRegisterEntry(S_004686a0* self, void* /*edx*/, int a2)
{
    // The original references S_0041e5f0's vtable (via the GrowButtonArray call site).
    (void)CSF_VTBL(0x0041E5F0);

    uint32_t* entry = nullptr;
    void* block = Mem_Alloc(0x84);
    if (block)
        entry = sub_467AD0(block, nullptr, a2);

    // Grow the list when count >= limit (unsigned compare, as in the original).
    if (CSF_AT(uint32_t, self, 0x30) >= CSF_AT(uint32_t, self, 0x2C))
        GrowButtonArray(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr, CSF_AT(int, self, 0x34));

    // Re-read count and array pointer after a possible grow.
    uint32_t index = CSF_AT(uint32_t, self, 0x30);
    uint32_t* items = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x28));
    uint32_t* slot = &items[index];
    CSF_AT(uint32_t, self, 0x30) = index + 1;
    *slot = reinterpret_cast<uint32_t>(entry);

    sub_468BB0(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
    return entry;
}

HOOK(0x468FD0, MissionScript_AllocAndRegisterEntry, "auto");
