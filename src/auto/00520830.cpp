#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, FxParticleManager_GrowInstanceCache, 0x0051FC20, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, InputBindings_FindEntryIndexByKey, 0x007686E0, (void* self, void* , int key));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSlotEntry, 0x00768740, (C_0085e7b8* self, void* , int slotIndex));

static int __fastcall S_005206d0_GetOrCreateParticleInstance(
    S_005206d0* self, void* /*edx*/, uint32_t* parameters)
{
    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* resources = CSF_AT(void*, manager, 0x1FCC);
    const int resourceIndex =
        ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, resources, 0)[0x10 / 4])(
                resources, nullptr, parameters[8]);

    manager = reinterpret_cast<void*>(GetValue8B0554());
    void* bindings = CSF_AT(void*, manager, 0x1F90);
    const int slotIndex = InputBindings_FindEntryIndexByKey(
        bindings, nullptr, static_cast<int>(parameters[9]));

    manager = reinterpret_cast<void*>(GetValue8B0554());
    resources = CSF_AT(void*, manager, 0x1FCC);
    void* resourceEntry =
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, resources, 0)[0x08 / 4])(
                resources, nullptr, resourceIndex);

    manager = reinterpret_cast<void*>(GetValue8B0554());
    auto* slotTable = CSF_AT(C_0085e7b8*, manager, 0x1F90);
    void* slotEntry = reinterpret_cast<void*>(
        C_0085e7b8_GetSlotEntry(slotTable, nullptr, slotIndex));

    const uint32_t cachedCount = self->field_0044;
    for (uint32_t index = 0; index < cachedCount; ++index)
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_003c) + 12 * index;
        if (CSF_AT(int, entry, 0x04) == resourceIndex &&
            CSF_AT(uint32_t, entry, 0x00) == parameters[11])
        {
            auto* matchedEntry =
                reinterpret_cast<uint8_t*>(self->field_003c) + 12 * index;
            void* controller = CSF_AT(void*, matchedEntry, 0x08);
            void* instance =
                ((void*(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, controller, 0)[0x0C / 4])(
                        controller, nullptr,
                        reinterpret_cast<uint8_t*>(slotEntry) + 4);

            const int option = static_cast<int>(parameters[10]);
            if (option != -1)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, instance, 0)[0x50 / 4])(
                        instance, nullptr, option);
            }
            return reinterpret_cast<int>(instance);
        }
    }

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
    void* object =
        ((void*(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, registry, 0)[0x1C / 4])(
                registry, nullptr,
                reinterpret_cast<uint8_t*>(resourceEntry) + 4, 1);

    void* controller = reinterpret_cast<void*>(
        FxParticleManager_GetParticleController(reinterpret_cast<int>(object)));
    void* instance =
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, controller, 0)[0x18 / 4])(
                controller, nullptr, 0);

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, instance, 0)[0x38 / 4])(
            instance, nullptr, reinterpret_cast<uint8_t*>(slotEntry) + 4);

    const int option = static_cast<int>(parameters[10]);
    if (option != -1)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, instance, 0)[0x50 / 4])(
                instance, nullptr, option);
    }

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr, 0);

    const uint32_t count = self->field_0044;
    const uint32_t capacity = self->field_0040;
    const uint32_t cacheKey = parameters[11];
    if (count >= capacity)
    {
        FxParticleManager_GrowInstanceCache(
            reinterpret_cast<uint8_t*>(self) + 0x3C, nullptr,
            CSF_AT(int, self, 0x48));
    }

    const uint32_t insertionIndex = self->field_0044;
    auto* entries = reinterpret_cast<uint8_t*>(self->field_003c);
    self->field_0044 = insertionIndex + 1;
    auto* entry = entries + 12 * insertionIndex;
    CSF_AT(uint32_t, entry, 0x00) = cacheKey;
    CSF_AT(int, entry, 0x04) = resourceIndex;
    CSF_AT(void*, entry, 0x08) = controller;
    return reinterpret_cast<int>(instance);
}

HOOK(0x00520830, S_005206d0_GetOrCreateParticleInstance, "auto");
