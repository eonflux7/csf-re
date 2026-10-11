#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Base0083e160_Finalize, 0x004E1320, (void* self, void* edx));

static int __fastcall C_0083e5e8_NotifyAndFinish(C_0083e5e8* self, void* /*edx*/)
{
    // Stack-resident sentinel object: vtable 0x82D448, then two -1 dwords.
    uint32_t sentinelObj[3];
    sentinelObj[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    sentinelObj[1] = 0xFFFFFFFFu;
    sentinelObj[2] = 0xFFFFFFFFu;

    // Virtual slot 0xCC / 4 = 51 (no typed wrapper for this slot).
    void** vtbl = CSF_AT(void**, self, 0);
    ((void(__fastcall*)(void*, void*, uint32_t*))vtbl[0xCC / 4])(self, nullptr, sentinelObj);

    return Base0083e160_Finalize(self, nullptr);
}

HOOK(0x004E23D0, C_0083e5e8_NotifyAndFinish, "auto");
