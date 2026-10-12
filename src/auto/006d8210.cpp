#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __fastcall, C_00861188_ResetInheritedState, 0x006D7F40,
         (void* self, void* edx));

static void* __fastcall C_00861188_ClearParticleResources(
    C_00861188* self, void* /*edx*/)
{
    C_00861188_ResetInheritedState(self, nullptr);

    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    CSF_AT(uint32_t, self, 0x130) = 0;
    CSF_AT(uint32_t, self, 0x134) = 0;
    CSF_AT(uint32_t, self, 0x138) = 0;

    for (uint32_t index = 0;
         index < CSF_AT(uint32_t, self, 0x100);
         ++index)
    {
        void* object = CSF_AT(void**, self, 0xF8)[index];
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x4 / 4])(
                    object, nullptr, 1);
            CSF_AT(void**, self, 0xF8)[index] = nullptr;
        }
    }

    void* objects = CSF_AT(void*, self, 0xF8);
    CSF_AT(uint32_t, self, 0xFC) = 0;
    CSF_AT(uint32_t, self, 0x100) = 0;
    if (objects)
    {
        Mem_Free(objects);
        CSF_AT(void*, self, 0xF8) = nullptr;
    }

    void* result = CSF_AT(void*, self, 0x108);
    CSF_AT(uint32_t, self, 0x10C) = 0;
    CSF_AT(uint32_t, self, 0x110) = 0;
    if (result)
    {
        result = reinterpret_cast<void*>(Mem_Free(result));
        CSF_AT(void*, self, 0x108) = nullptr;
    }

    CSF_AT(uint8_t, self, 0x99) = 0;
    return result;
}

HOOK(0x006D8210, C_00861188_ClearParticleResources, "auto");
