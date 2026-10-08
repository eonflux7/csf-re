#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x00548B70 (20 bytes) — CMenuSoundOptionsPC
// Forwards the Sound Blaster X-Fi option byte (+0x68) to the helper object held at
// +0x2C through its virtual slot 0x3C. Does nothing when that object is null.
static int __fastcall CMenuSoundOptionsPC_ApplyXfiSetting(CMenuSoundOptionsPC* self, void* /*edx*/)
{
    uint32_t helper = self->field_002c;
    if (helper == 0)
        return static_cast<int>(helper);

    void* obj = reinterpret_cast<void*>(helper);
    int (*fn)(void*, void*, int) =
        reinterpret_cast<int (*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[0x3C / 4]);
    return fn(obj, nullptr, static_cast<int>(self->field_0068));
}

HOOK(0x00548B70, CMenuSoundOptionsPC_ApplyXfiSetting, "auto");
