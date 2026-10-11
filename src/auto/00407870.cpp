#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_407490, 0x00407490, (void* self, void* edx, float a2, int a3));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0082d77c_UpdateActiveSlots(C_0082d77c* self, void* /*edx*/, float deltaTime)
{
    // owner = self->field_0008; first = owner->vtbl[0x28/4](owner)
    C_0082d410* owner = reinterpret_cast<C_0082d410*>(self->field_0008);
    void* first = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    // second = first->vtbl[0x68/4](first)
    void* second = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, first, 0)[0x68 / 4])(first, nullptr);

    // enabled = second->vtbl[0x48/4](second)
    bool enabled = ((bool (__fastcall*)(void*, void*))CSF_AT(void**, second, 0)[0x48 / 4])(second, nullptr);

    if (enabled && deltaTime > g_fZero)
    {
        int slotIndex = 0;
        for (uint32_t offset = 0; offset < 0x84; offset += 44)
        {
            if (*(reinterpret_cast<uint8_t*>(self->field_000c) + offset))
                sub_407490(self, nullptr, deltaTime, slotIndex);
            ++slotIndex;
        }
    }
}

HOOK(0x00407870, C_0082d77c_UpdateActiveSlots, "auto");
