#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_008443c4_Constructor, 0x0054D850, (C_008443c4* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, SceneLoader_DestroyReferenceRegistry, 0x0069ACF0, (void* self, void* edx));

static int __fastcall CSaveGameManager_ReleaseSaveResources(
    CSaveGameManager* self, void* /*edx*/)
{
    int result = 0;

    if (void* object = reinterpret_cast<void*>(self->field_002c))
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_002c = 0;
    }

    if (void* object = reinterpret_cast<void*>(self->field_0028))
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0028 = 0;
    }

    if (void* object = reinterpret_cast<void*>(self->field_001c))
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_001c = 0;
    }

    if (void* object = reinterpret_cast<void*>(self->field_0024))
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0024 = 0;
    }

    if (void* object = reinterpret_cast<void*>(self->field_0020))
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0020 = 0;
    }

    if (auto* object = reinterpret_cast<C_008443c4*>(self->field_0030))
    {
        C_008443c4_Constructor(object, nullptr);
        result = Mem_Free(object);
        self->field_0030 = 0;
    }

    if (void* object = reinterpret_cast<void*>(self->field_0034))
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0034 = 0;
    }

    if (void* object = reinterpret_cast<void*>(self->field_0038))
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0038 = 0;
    }

    if (void* registry = reinterpret_cast<void*>(self->field_003c))
    {
        SceneLoader_DestroyReferenceRegistry(registry, nullptr);
        result = Mem_Free(registry);
        self->field_003c = 0;
    }

    return result;
}

HOOK(0x00511FF0, CSaveGameManager_ReleaseSaveResources, "auto");
