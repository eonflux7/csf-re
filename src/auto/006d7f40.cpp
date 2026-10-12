#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_008611d4_ResetInheritedState(
    C_008611d4* self, void* /*edx*/)
{
    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0xD0); ++i)
    {
        void* child = CSF_AT(void**, self, 0xC8)[i];
        if (child)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[1])(child, nullptr, 1);
            CSF_AT(void**, self, 0xC8)[i] = nullptr;
        }
    }

    void* block = CSF_AT(void*, self, 0xC8);
    CSF_AT(uint32_t, self, 0xCC) = 0;
    CSF_AT(uint32_t, self, 0xD0) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xC8) = nullptr;
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0xB0); ++i)
    {
        void* child = CSF_AT(void**, self, 0xA8)[i];
        if (child)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[1])(child, nullptr, 1);
            CSF_AT(void**, self, 0xA8)[i] = nullptr;
        }
    }

    block = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint32_t, self, 0xAC) = 0;
    CSF_AT(uint32_t, self, 0xB0) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xA8) = nullptr;
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0xC0); ++i)
    {
        void* child = CSF_AT(void**, self, 0xB8)[i];
        if (child)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[1])(child, nullptr, 1);
            CSF_AT(void**, self, 0xB8)[i] = nullptr;
        }
    }

    block = CSF_AT(void*, self, 0xB8);
    CSF_AT(uint32_t, self, 0xBC) = 0;
    CSF_AT(uint32_t, self, 0xC0) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xB8) = nullptr;
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0xE0); ++i)
    {
        void* child = CSF_AT(void**, self, 0xD8)[i];
        if (child)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[1])(child, nullptr, 1);
            CSF_AT(void**, self, 0xD8)[i] = nullptr;
        }
    }

    void* result = CSF_AT(void*, self, 0xD8);
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    if (result)
    {
        result = reinterpret_cast<void*>(Mem_Free(result));
        CSF_AT(void*, self, 0xD8) = nullptr;
    }
    return result;
}

HOOK(0x006D7F40, C_008611d4_ResetInheritedState, "auto");
