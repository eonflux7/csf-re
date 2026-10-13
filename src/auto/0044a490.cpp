#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, DestroyElementArrays, 0x004490D0, (int begin, int end, int , int));
CSF_FUNC(void, __fastcall, C_0083114c_ResetSubsystemC4, 0x0044A250, (C_0083114c* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall C_0083114c_Cleanup(C_0083114c* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083114C);
    C_0083114c_ResetSubsystemC4(self, nullptr);

    auto* elements = reinterpret_cast<uint8_t*>(self->field_003c);
    if (elements)
    {
        auto* allocation = elements - 4;
        uint32_t count = CSF_AT(uint32_t, allocation, 0);
        auto* cursor = elements + 16 * count;

        if (static_cast<int32_t>(count - 1) >= 0)
        {
            do
            {
                cursor -= 0x10;
                --count;
                CSF_AT(void**, cursor, 0) = CSF_VTBL(0x0082D410);
            } while (count);
        }

        CoreMemory_Free(allocation);
        self->field_003c = 0;
    }

    auto* objects = reinterpret_cast<C_0082d410*>(self->field_002c);
    if (objects)
    {
        auto* allocation = reinterpret_cast<uint8_t*>(objects) - 4;
        if (CSF_AT(uint32_t, allocation, 0))
            objects->vSlot0(3);
        else
            CoreMemory_Free(allocation);

        self->field_002c = 0;
    }

    const uint32_t arrayBegin = self->field_0018;
    if (arrayBegin)
    {
        DestroyElementArrays(
            static_cast<int>(arrayBegin),
            CSF_AT(int, self, 0x1C),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x14),
            0);

        if (self->field_0018)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
    }

    self->field_0018 = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;

    const uint32_t pairedArrayAddress = self->field_0004;
    int result = static_cast<int>(pairedArrayAddress);
    if (pairedArrayAddress)
    {
        auto* pairedElements = reinterpret_cast<uint8_t*>(pairedArrayAddress);
        auto* allocation = pairedElements - 4;
        uint32_t count = CSF_AT(uint32_t, allocation, 0);
        auto* cursor = pairedElements + 8 * (3 * count);

        if (static_cast<int32_t>(count - 1) >= 0)
        {
            do
            {
                cursor -= 0x18;
                --count;
                CSF_AT(void**, cursor, 0x0C) = CSF_VTBL(0x0082D410);
                CSF_AT(void**, cursor, 0) = CSF_VTBL(0x0082D410);
            } while (count);
        }

        result = CoreMemory_Free(allocation);
        self->field_0004 = 0;
    }

    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0044A490, C_0083114c_Cleanup, "auto");
