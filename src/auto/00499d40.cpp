#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833b88_Destructor, 0x004920F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall C_00833ee8_Destructor(C_00833ee8* self, void* /*edx*/)
{
    const uint8_t hadAttachedObject = self->field_0228;

    self->vtbl = CSF_VTBL(0x00833EE8);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x00833E68);
    CSF_AT(uint32_t, self, 0x8) = (uint32_t)CSF_ADDR_DATA(0x00833E60);
    self->field_000c = (uint32_t)CSF_ADDR_DATA(0x00833E54);
    CSF_AT(uint32_t, self, 0x10) = (uint32_t)CSF_ADDR_DATA(0x00833E4C);
    self->field_0014 = (uint32_t)CSF_ADDR_DATA(0x00833E48);

    if (hadAttachedObject)
    {
        void* owner = reinterpret_cast<void*>(self->field_0044);
        void* attached = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0xD8 / 4])(owner, nullptr);
        if (attached)
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, attached, 0)[0x3C / 4])(attached, nullptr);
    }

    if (self->field_022c)
    {
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, sceneController, 0)[0x18 / 4])(
            sceneController, nullptr, self->field_022c);
        self->field_022c = 0;
    }

    if (self->field_039c)
    {
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, sceneController, 0)[0x18 / 4])(
            sceneController, nullptr, self->field_039c);
        self->field_039c = 0;
    }

    if (self->field_03e8)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_03e8));
        self->field_03e8 = 0;
    }

    // delete[] of an array of polymorphic objects (element count stored just before the array)
    void* objectArray = reinterpret_cast<void*>(self->field_03f0);
    if (objectArray)
    {
        void* arrayHeader = static_cast<uint8_t*>(objectArray) - 4;
        if (CSF_AT(uint32_t, arrayHeader, 0) != 0)
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, objectArray, 0)[0])(objectArray, nullptr, 3);
        else
            Mem_Free(arrayHeader);
        self->field_03f0 = 0;
    }

    self->field_03c0 = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_03b4 = (uint32_t)CSF_VTBL(0x0082D410);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x344), nullptr);
    self->field_0324 = (uint32_t)CSF_VTBL(0x0082D410);

    if (self->field_0310)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0310));
    self->field_0310 = 0;
    self->field_0314 = 0;
    self->field_0318 = 0;

    if (self->field_0300)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0300));
    self->field_0300 = 0;
    self->field_0304 = 0;
    self->field_0308 = 0;

    self->field_02d4 = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_02c8 = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_02b4 = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_029c = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_028c = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_0270 = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_024c = (uint32_t)CSF_VTBL(0x0082D410);
    self->field_0240 = (uint32_t)CSF_VTBL(0x0082D410);

    return C_00833b88_Destructor(self, nullptr);
}

HOOK(0x00499D40, C_00833ee8_Destructor, "auto");
