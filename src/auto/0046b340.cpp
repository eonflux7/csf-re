#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

struct CMissionScript;

CSF_FUNC(int, __cdecl, sub_468AF0, 0x00468AF0, (int* a1, int* a2, int a3, int a4));
CSF_FUNC(int, __cdecl, sub_469710, 0x00469710, (int a1, int a2));
CSF_FUNC(int, __fastcall, Scene_ResetSubsystemBC, 0x0046A7A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_46B1F0, 0x0046B1F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall CMissionScript_Destroy(CMissionScript* self, void* /*edx*/)
{
    CSF_AT(void**, self, 0x00) = CSF_VTBL(0x008314F8);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x008314DC);
    Scene_ResetSubsystemBC(self, nullptr);
    sub_46B1F0(self, nullptr);

    void* object = CSF_AT(void*, self, 0x38);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        CSF_AT(void*, self, 0x38) = nullptr;
    }

    object = CSF_AT(void*, self, 0xA8);
    if (object)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        CSF_AT(void*, self, 0xA8) = nullptr;
    }

    void* block = CSF_AT(void*, self, 0xC0);
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xC0) = nullptr;
    }

    block = CSF_AT(void*, self, 0xB0);
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0xB0) = nullptr;
    }

    CSF_AT(void**, self, 0x94) = CSF_VTBL(0x008314C0);
    block = CSF_AT(void*, self, 0x9C);
    if (block)
        Mem_FreeDebug(block);
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(void**, self, 0x94) = CSF_VTBL(0x0082D410);

    object = CSF_AT(void*, self, 0x84);
    if (object)
    {
        if (CSF_AT(uint32_t, object, -4))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 3);
        }
        else
        {
            Mem_Free(static_cast<uint8_t*>(object) - 4);
        }
        CSF_AT(void*, self, 0x84) = nullptr;
    }

    int* begin = CSF_AT(int*, self, 0x78);
    if (begin)
    {
        sub_468AF0(begin, CSF_AT(int*, self, 0x7C),
                   reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x74),
                   0);
        block = CSF_AT(void*, self, 0x78);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;
    CSF_AT(uint32_t, self, 0x80) = 0;

    begin = CSF_AT(int*, self, 0x68);
    if (begin)
    {
        sub_468AF0(begin, CSF_AT(int*, self, 0x6C),
                   reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64),
                   0);
        block = CSF_AT(void*, self, 0x68);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;

    CSF_AT(uint32_t, self, 0x50) = 0;
    block = CSF_AT(void*, self, 0x58);
    if (block)
        Mem_FreeDebug(block);
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x60) = 0;

    block = CSF_AT(void*, self, 0x44);
    if (block)
        Mem_FreeDebug(block);
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;

    block = CSF_AT(void*, self, 0x28);
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x28) = nullptr;
    }

    block = CSF_AT(void*, self, 0x18);
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x18) = nullptr;
    }

    int result = CSF_AT(int, self, 0x0C);
    if (result)
    {
        sub_469710(result, CSF_AT(int, self, 0x10));
        result = CSF_AT(int, self, 0x0C);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0046B340, CMissionScript_Destroy, "auto");
