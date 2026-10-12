#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DevConsole_ResetState, 0x004F91C0, (void* self, void* , char preserveSecondaryState));
CSF_FUNC(void, __fastcall, SceneLoader_PrepareGlobalSceneState, 0x004FC440, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

static int __fastcall C_0083f448_PrepareSceneManager(
    C_0083f448* self, void* /*edx*/)
{
    SceneLoader_PrepareGlobalSceneState(self, nullptr);
    DevConsole_ResetState(reinterpret_cast<void*>(self->field_0074), nullptr, 0);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0074), 0x04) = 0;

    if (self->field_0058)
    {
        void* entry = nullptr;
        if (self->field_001c)
            entry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0);

        void* lookup = reinterpret_cast<void*>(ResolveEntryLookupValue(entry, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, lookup, 0)[0x3C / 4])(
                lookup, nullptr, self->field_0058);

        int managerId = GetGlobalInt();
        void* manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, manager, 0)[0x24 / 4])(
                manager, nullptr, self->field_0058);
        self->field_0058 = 0;
    }

    if (self->field_005c)
    {
        void* entry = nullptr;
        if (self->field_001c)
            entry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0);

        void* lookup = reinterpret_cast<void*>(ResolveEntryLookupValue(entry, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, lookup, 0)[0x3C / 4])(
                lookup, nullptr, self->field_005c);

        int managerId = GetGlobalInt();
        void* manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, manager, 0)[0x24 / 4])(
                manager, nullptr, self->field_005c);
        self->field_005c = 0;
    }

    if (self->field_006c)
    {
        void* entry = nullptr;
        if (self->field_001c)
            entry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0);

        void* lookup = reinterpret_cast<void*>(ResolveEntryLookupValue(entry, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, lookup, 0)[0x3C / 4])(
                lookup, nullptr, self->field_006c);

        int managerId = GetGlobalInt();
        void* manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, manager, 0)[0x24 / 4])(
                manager, nullptr, self->field_006c);
        self->field_006c = 0;
    }

    for (uint32_t index = 0; index < self->field_001c; ++index)
    {
        void* entry = reinterpret_cast<void**>(self->field_0014)[index];
        if (entry)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0014)[index] = 0;
        }
    }

    void* entries = reinterpret_cast<void*>(self->field_0014);
    self->field_0018 = 0;
    self->field_001c = 0;
    if (entries)
    {
        Mem_Free(entries);
        self->field_0014 = 0;
    }

    void* sceneObject = reinterpret_cast<void*>(self->field_0034);
    if (sceneObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, sceneObject, 0)[0])(sceneObject, nullptr, 1);
        self->field_0034 = 0;
    }

    for (uint32_t index = 0; index < self->field_000c; ++index)
    {
        void* entry = reinterpret_cast<void**>(self->field_0004)[index];
        if (entry)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0004)[index] = 0;
        }
    }

    int result = static_cast<int>(self->field_0004);
    self->field_0008 = 0;
    self->field_000c = 0;
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(result));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x004FC550, C_0083f448_PrepareSceneManager, "auto");
