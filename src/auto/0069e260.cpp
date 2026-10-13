#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, RangeList_ContainsValue, 0x0069B320, (void* self, void* /*edx*/, uint32_t value));
CSF_FUNC(uint32_t, __cdecl, __msize, 0x007B2FB8, (void* Block));

static size_t __fastcall C_0085eac0_RoundBlockSizeToClass(C_0085eac0* self, void* /*edx*/, void* Block)
{
    if (!Block)
        return 0;

    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    const uint32_t blockValue = reinterpret_cast<uint32_t>(Block);

    if (RangeList_ContainsValue(base + 0x08, nullptr, blockValue))
        return 4;
    if (RangeList_ContainsValue(base + 0x4C, nullptr, blockValue))
        return 8;
    if (RangeList_ContainsValue(base + 0x90, nullptr, blockValue))
        return 16;
    if (RangeList_ContainsValue(base + 0xD4, nullptr, blockValue))
        return 32;

    return __msize(Block);
}

HOOK(0x0069E260, C_0085eac0_RoundBlockSizeToClass, "auto");
