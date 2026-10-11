#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ResetRecordRange, 0x006DEC40,
         (int* begin, int* end, ...));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int, __fastcall, C_00871a18_Teardown_7741C0, 0x007741C0,
         (void* self, void* edx));

static int __fastcall C_00840704_Destroy(C_00840704* self, void* /*edx*/)
{
    // Records block lives at this+0x8 (pad_0008 in the declared layout).
    auto* records = reinterpret_cast<std::uint8_t*>(self) + 0x8;

    self->vtbl = CSF_VTBL(0x00840704);

    const std::uint32_t begin = self->field_000c;
    if (begin)
    {
        ResetRecordRange(
            reinterpret_cast<int*>(begin),
            reinterpret_cast<int*>(CSF_AT(std::uint32_t, records, 8)),
            records,
            self);

        const std::uint32_t allocation = CSF_AT(std::uint32_t, records, 4);
        if (allocation)
            Mem_FreeDebug(reinterpret_cast<void*>(allocation));
    }

    CSF_AT(std::uint32_t, records, 4) = 0;
    CSF_AT(std::uint32_t, records, 8) = 0;
    CSF_AT(std::uint32_t, records, 0xC) = 0;

    return C_00871a18_Teardown_7741C0(self, nullptr);
}

HOOK(0x005105F0, C_00840704_Destroy, "auto");
