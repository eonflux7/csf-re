#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall C_008316d8_ReleaseResources(C_008316d8* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0038);
    if (result)
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(global, nullptr));
        result = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, registry, 0)[6])(
                registry, nullptr, self->field_0038);
        self->field_0038 = 0;
    }

    void* object = reinterpret_cast<void*>(self->field_0028);
    if (object)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0028 = 0;
    }

    void* block = reinterpret_cast<void*>(self->field_0030);
    if (block)
    {
        SceneLoader_NoOp();
        result = Mem_Free(block);
        self->field_0030 = 0;
    }
    return result;
}

HOOK(0x00475AD0, C_008316d8_ReleaseResources, "auto");
