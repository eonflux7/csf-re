#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, HAnim_ComputeFrameIndex, 0x006E82F0, (int a1, int a2));

static int __fastcall C_00861e98_DispatchWithResolvedIndex(C_00861e98* self, void* /*edx*/, int a2)
{
    if (CSF_AT(int, self, 0x1C) == 0)
        return 0;

    int resolvedIndex = HAnim_ComputeFrameIndex(CSF_AT(int, self, 0x14), a2);

    // Virtual slot 0x6C of the object's vtable; the vtable is re-read after the call, as the original does.
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr, resolvedIndex);
}

HOOK(0x006E8A10, C_00861e98_DispatchWithResolvedIndex, "auto");
