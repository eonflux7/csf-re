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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, S_004a31e0_AssignTextEntryVector, 0x004EFF30, (S_004a31e0* self, void* , S_004a31e0* source));
CSF_FUNC(unsigned int*, __fastcall, S_006b0320_PrepareAndApply, 0x006B0B70, (void* self, void* , int a2));

// Sound-area records are 0x68 bytes. The range [first, last) is copied backwards
// so that the last record lands just before destEnd; the new destination start is returned.
static int* __cdecl SoundAreas_CopyAreaRangeBackward(int* first, int* last, int* destEnd, int /*unused*/)
{
    if (first == last)
        return destEnd;

    uint8_t* src = reinterpret_cast<uint8_t*>(last);
    uint8_t* dst = reinterpret_cast<uint8_t*>(destEnd);
    do {
        src -= 0x68;
        dst -= 0x68;

        CSF_AT(int, dst, 0x00) = CSF_AT(int, src, 0x00);

        AssignStringSubstring(dst + 0x04, nullptr, src + 0x04, 0, 0xFFFFFFFFu);
        S_004a31e0_AssignTextEntryVector(reinterpret_cast<S_004a31e0*>(dst + 0x20), nullptr,
                                         reinterpret_cast<S_004a31e0*>(src + 0x20));
        S_006b0320_PrepareAndApply(dst + 0x30, nullptr,
                                   static_cast<int>(reinterpret_cast<std::uintptr_t>(src + 0x30)));

        CSF_AT(int, dst, 0x50) = CSF_AT(int, src, 0x50);
        CSF_AT(int, dst, 0x54) = CSF_AT(int, src, 0x54);
        CSF_AT(int, dst, 0x58) = CSF_AT(int, src, 0x58);
        CSF_AT(int, dst, 0x5C) = CSF_AT(int, src, 0x5C);
        CSF_AT(int, dst, 0x60) = CSF_AT(int, src, 0x60);
        CSF_AT(int, dst, 0x64) = CSF_AT(int, src, 0x64);
    } while (src != reinterpret_cast<uint8_t*>(first));

    return reinterpret_cast<int*>(dst);
}

HOOK(0x00770F00, SoundAreas_CopyAreaRangeBackward, "auto");
