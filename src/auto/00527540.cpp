#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void __fastcall CSoundInstance_DestroyContents(
    CSoundInstance* self, void* /*edx*/)
{
    void* voice = reinterpret_cast<void*>(self->m_voice);
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x008410B0));

    if (voice)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, voice, 0)[0])(voice, nullptr, 1);
        self->m_voice = 0;
    }

    self->m_entityHandle = reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x0082D410));
    self->field_0028 = reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x0082D410));
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x0082D410));
}

static uint32_t* __fastcall CSoundInstance_DeletingDestructor(
    CSoundInstance* self, void* /*edx*/, char flags)
{
    if ((flags & 2) != 0)
    {
        uint32_t count = CSF_AT(uint32_t, self, -4);
        auto* allocation = reinterpret_cast<uint8_t*>(self) - 4;
        auto* end = reinterpret_cast<uint8_t*>(self) + count * 0x70;

        if (static_cast<int32_t>(count - 1) >= 0)
        {
            do
            {
                end -= 0x70;
                CSoundInstance_DestroyContents(
                    reinterpret_cast<CSoundInstance*>(end), nullptr);
                --count;
            }
            while (count != 0);
        }

        if ((flags & 1) != 0)
            Mem_Free(allocation);

        return reinterpret_cast<uint32_t*>(allocation);
    }

    CSoundInstance_DestroyContents(self, nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00527540, CSoundInstance_DeletingDestructor, "auto");
