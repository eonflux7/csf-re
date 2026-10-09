#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyVariableRange, 0x00469710, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(void, __fastcall, SceneLoader_RemoveObjectEntry, 0x00475480, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall CMissionScript_Reset(C_008314f8* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0xD0) = 0;
    void* buffer = CSF_AT(void*, self, 0xC0);
    CSF_AT(uint32_t, self, 0xC4) = 0;
    CSF_AT(uint32_t, self, 0xC8) = 0;
    if (buffer)
    {
        Mem_Free(buffer);
        CSF_AT(void*, self, 0xC0) = nullptr;
    }

    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x30); ++index)
    {
        void* object = CSF_AT(void**, self, 0x28)[index];
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            CSF_AT(void**, self, 0x28)[index] = nullptr;
        }
    }

    buffer = CSF_AT(void*, self, 0x28);
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    if (buffer)
    {
        Mem_Free(buffer);
        CSF_AT(void*, self, 0x28) = nullptr;
    }

    uint8_t* entry = CSF_AT(uint8_t*, self, 0x68);
    if (entry)
    {
        uint8_t* end = CSF_AT(uint8_t*, self, 0x6C);
        for (; entry != end; entry += 0x10)
        {
            void* allocation = CSF_AT(void*, entry, 0);
            if (allocation)
            {
                Mem_Free(allocation);
                CSF_AT(void*, entry, 0) = nullptr;
            }
        }
        buffer = CSF_AT(void*, self, 0x68);
        if (buffer)
            Mem_FreeDebug(buffer);
    }
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;

    entry = CSF_AT(uint8_t*, self, 0x78);
    if (entry)
    {
        uint8_t* end = CSF_AT(uint8_t*, self, 0x7C);
        for (; entry != end; entry += 0x10)
        {
            void* allocation = CSF_AT(void*, entry, 0);
            if (allocation)
            {
                Mem_Free(allocation);
                CSF_AT(void*, entry, 0) = nullptr;
            }
        }
        buffer = CSF_AT(void*, self, 0x78);
        if (buffer)
            Mem_FreeDebug(buffer);
    }
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;
    CSF_AT(uint32_t, self, 0x80) = 0;

    CSF_AT(uint8_t, self, 0xAC) = 0;
    buffer = CSF_AT(void*, self, 0xB0);
    CSF_AT(uint32_t, self, 0xB4) = 0;
    CSF_AT(uint32_t, self, 0xB8) = 0;
    if (buffer)
    {
        Mem_Free(buffer);
        CSF_AT(void*, self, 0xB0) = nullptr;
    }

    void* objects = CSF_AT(void*, self, 0x84);
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    if (objects)
    {
        uint8_t* header = static_cast<uint8_t*>(objects) - 4;
        if (CSF_AT(uint32_t, header, 0))
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, objects, 0)[0])(objects, nullptr, 3);
        }
        else
        {
            Mem_Free(header);
        }
        CSF_AT(void*, self, 0x84) = nullptr;
    }

    buffer = CSF_AT(void*, self, 0x9C);
    if (buffer)
        Mem_FreeDebug(buffer);
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;

    void* object = CSF_AT(void*, self, 0x38);
    if (object)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        CSF_AT(void*, self, 0x38) = nullptr;
    }

    int variablesBegin = CSF_AT(int, self, 0x0C);
    if (variablesBegin)
    {
        MissionScript_DestroyVariableRange(
            variablesBegin, CSF_AT(int, self, 0x10));
        buffer = CSF_AT(void*, self, 0x0C);
        if (buffer)
            Mem_FreeDebug(buffer);
    }
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    void* registry = reinterpret_cast<void*>(GetGlobalObjectField184());

    // The original exposes the removal routine's residual EAX as its return.
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        SceneLoader_RemoveObjectEntry)(
            registry, nullptr, reinterpret_cast<int>(self));
}

HOOK(0x0046A7A0, CMissionScript_Reset, "auto");
