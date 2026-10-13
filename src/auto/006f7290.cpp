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

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, IsBit5Set, 0x006B8190, (void* self, void*));

// Returns the input handle when the object is flagged and its attachment has bit 5 set
// (or when the flag is clear); otherwise sets *outFlag = 1 and returns 0.
static int __cdecl GfxObjectProps_CheckFlagAndMarkIfNoBit5(int objectHandle, uint8_t* outFlag) {
    uint8_t* objectBytes = reinterpret_cast<uint8_t*>(objectHandle);

    if ((CSF_AT(uint8_t, objectBytes, 0x02) & 1) == 0)
        return objectHandle;

    int objectAttachment = GfxObjectProps_GetObjectAttachment(objectHandle);
    if ((uint8_t)IsBit5Set(reinterpret_cast<void*>(objectAttachment), nullptr))
        return objectHandle;

    *outFlag = 1;
    return 0;
}

HOOK(0x6F7290, GfxObjectProps_CheckFlagAndMarkIfNoBit5, "auto");
