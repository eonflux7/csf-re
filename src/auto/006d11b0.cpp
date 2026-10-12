#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxLights_ClearClumpManager(void* self, void* /*edx*/)
{
    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x3C); ++i)
    {
        void* object = CSF_AT(void**, self, 0x34)[i];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x34)[i] = nullptr;
        }
    }

    void* block = CSF_AT(void*, self, 0x34);
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x34) = nullptr;
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x4C); ++i)
    {
        void* object = CSF_AT(void**, self, 0x44)[i];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x44)[i] = nullptr;
        }
    }

    block = CSF_AT(void*, self, 0x44);
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x44) = nullptr;
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x6C); ++i)
    {
        void* object = CSF_AT(void**, self, 0x64)[i];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x64)[i] = nullptr;
        }
    }

    block = CSF_AT(void*, self, 0x64);
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x64) = nullptr;
    }

    block = CSF_AT(void*, self, 0x54);
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x54) = nullptr;
    }

    block = CSF_AT(void*, self, 0x74);
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x74) = nullptr;
    }

    block = CSF_AT(void*, self, 0x88);
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x90) = 0;
    int result = 0;
    if (block)
    {
        result = Mem_Free(block);
        CSF_AT(void*, self, 0x88) = nullptr;
    }
    return result;
}

HOOK(0x006D11B0, GfxLights_ClearClumpManager, "auto");
