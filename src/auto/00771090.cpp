#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, S_004a31e0_AssignTextEntryVector, 0x004EFF30,
         (S_004a31e0* self, void* edx, S_004a31e0* source));
CSF_FUNC(unsigned int*, __fastcall, S_006b0320_PrepareAndApply, 0x006B0B70,
         (void* self, void* edx, int a2));

// Fills every 104-byte sound-area record in [firstRecord, endRecord) from the
// parsed source entry. Returns endRecord, as the original does.
static int __cdecl SoundAreas_FillRecordRange(int firstRecord, int endRecord, S_004a31e0* source)
{
    int result = endRecord;
    for (int record = firstRecord; record != endRecord; record += 104)
    {
        auto* dst = reinterpret_cast<uint8_t*>(record);
        auto* src = reinterpret_cast<uint8_t*>(source);

        CSF_AT(uint32_t, dst, 0) = CSF_AT(uint32_t, src, 0);
        AssignStringSubstring(dst + 4, nullptr, src + 4, 0, static_cast<std::uint32_t>(-1));
        S_004a31e0_AssignTextEntryVector(reinterpret_cast<S_004a31e0*>(dst + 32), nullptr,
                                         reinterpret_cast<S_004a31e0*>(src + 0x20));
        S_006b0320_PrepareAndApply(dst + 48, nullptr, reinterpret_cast<int>(src + 0x30));

        CSF_AT(uint32_t, dst, 80) = CSF_AT(uint32_t, src, 0x50);
        CSF_AT(uint32_t, dst, 84) = CSF_AT(uint32_t, src, 0x54);
        CSF_AT(uint32_t, dst, 88) = CSF_AT(uint32_t, src, 0x58);
        CSF_AT(uint32_t, dst, 92) = CSF_AT(uint32_t, src, 0x5C);
        CSF_AT(uint32_t, dst, 96) = CSF_AT(uint32_t, src, 0x60);
        CSF_AT(uint32_t, dst, 100) = CSF_AT(uint32_t, src, 0x64);

        result = endRecord;
    }
    return result;
}

HOOK(0x00771090, SoundAreas_FillRecordRange, "auto");
