#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __cdecl, CoreParser_ReleaseAllocation, 0x00692A70, (S_0069b1c0* allocation));
CSF_FUNC(char, __fastcall, sub_693370, 0x00693370, (void* self, void* edx));

static S_0069b1c0** __fastcall S_0069b1c0_ResetAndMaybeFree(S_0069b1c0** self, void* /*edx*/, char freeSelf)
{
    if (CSF_AT(uint8_t, self, 0x0B) == 0xFF)
    {
        S_0069b1c0* owned = self[0];
        if (owned)
        {
            sub_693370(owned, nullptr);
            CoreParser_ReleaseAllocation(owned);
            self[0] = nullptr;
        }
    }

    CSF_AT(uint8_t, self, 0x0B) = 0xFF;

    if ((freeSelf & 1) != 0)
    {
        CoreMemory_Free(self);
    }

    return self;
}

HOOK(0x693B80, S_0069b1c0_ResetAndMaybeFree, "auto");
