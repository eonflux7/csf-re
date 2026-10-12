#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall S_00493c70_IsFlag1000Allowed(S_00493c70* self, void* /*edx*/)
{
    // Any nonzero byte at +0x3DE vetoes the result.
    const uint8_t flag03de = self->field_03de;
    if (flag03de != 0) {
        return false;
    }

    // Both status words must have none of the blocking bits set.
    if ((self->field_02e4 & 0xFFFED7FBu) != 0) {
        return false;
    }

    const uint32_t statusWord = self->field_02e0;
    if ((statusWord & 0xFFFED7FBu) != 0) {
        return false;
    }

    // Without bit 0x2000 the object is allowed outright.
    if ((statusWord & 0x2000u) == 0) {
        return true;
    }

    // Otherwise the handle at this+0x1B4 must resolve to an object whose type is 7.
    uint8_t* handleAddress = reinterpret_cast<uint8_t*>(self) + 0x1B4;
    const int handle = static_cast<int>(reinterpret_cast<uintptr_t>(handleAddress));
    const int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (resolved == 0) {
        return false;
    }

    void* resolvedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(resolved));
    return CSF_AT(int, resolvedObject, 0x50) == 7;
}

HOOK(0x00493C70, S_00493c70_IsFlag1000Allowed, "auto");
