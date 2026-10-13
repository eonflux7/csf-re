#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, DestroyElementArray_445F40, 0x00445F40, (void* self, void*));
CSF_FUNC(void, __fastcall, Scene_ClearEmbeddedState, 0x00445F90, (void* self, void*));
CSF_FUNC(int, __fastcall, ResetBufferAndReleaseEntries, 0x0044DE50, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(void, __fastcall, SceneLoader_RemoveObjectEntry, 0x00475480, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static void __fastcall C_00831120_ResetSubsystemState(
    C_00831120* self, void* /*edx*/)
{
    self->field_00e8 = 0;
    DestroyElementArray_445F40(&self->field_00d8, nullptr);

    self->field_00fc = 0;
    Scene_ClearEmbeddedState(&self->field_00ec, nullptr);

    self->field_0110 = 0;
    void* buffer = reinterpret_cast<void*>(self->field_0100);
    self->field_0104 = 0;
    self->field_0108 = 0;
    if (buffer)
    {
        CoreMemory_Free(buffer);
        self->field_0100 = 0;
    }

    DestroyElementArray_445F40(&self->field_000c, nullptr);
    Scene_ClearEmbeddedState(&self->field_001c, nullptr);

    buffer = reinterpret_cast<void*>(self->field_002c);
    self->field_0030 = 0;
    self->field_0034 = 0;
    if (buffer)
    {
        CoreMemory_Free(buffer);
        self->field_002c = 0;
    }

    void* elements = reinterpret_cast<void*>(self->field_0040);
    self->field_0044 = 0;
    self->field_0048 = 0;
    if (elements)
    {
        if (CSF_AT(uint32_t, elements, -0x4))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, elements, 0)[0])(elements, nullptr, 3);
        }
        else
        {
            CoreMemory_Free(static_cast<uint8_t*>(elements) - 4);
        }
        self->field_0040 = 0;
    }

    elements = reinterpret_cast<void*>(self->field_0050);
    self->field_0054 = 0;
    self->field_0058 = 0;
    if (elements)
    {
        if (CSF_AT(uint32_t, elements, -0x4))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, elements, 0)[0])(elements, nullptr, 3);
        }
        else
        {
            CoreMemory_Free(static_cast<uint8_t*>(elements) - 4);
        }
        self->field_0050 = 0;
    }

    elements = reinterpret_cast<void*>(self->field_00c8);
    self->field_00cc = 0;
    self->field_00d0 = 0;
    if (elements)
    {
        if (CSF_AT(uint32_t, elements, -0x4))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, elements, 0)[0])(elements, nullptr, 3);
        }
        else
        {
            CoreMemory_Free(static_cast<uint8_t*>(elements) - 4);
        }
        self->field_00c8 = 0;
    }

    ResetBufferAndReleaseEntries(
        reinterpret_cast<void*>(self->field_0114), nullptr);

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalObjectField184());
    SceneLoader_RemoveObjectEntry(
        sceneLoader, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
}

HOOK(0x00446210, C_00831120_ResetSubsystemState, "auto");
