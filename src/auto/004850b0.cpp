#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_SetObjectTransform, 0x00490790, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, LookupStateEntry, 0x004E9540, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, GetProviderStateIndex, 0x0051AEC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetStateAuxiliaryValue, 0x0052D5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetStateAdditionalValue, 0x0070E390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static char __stdcall CharActionFsm_ApplyStateEntryTransform(
    C_0085e7b8* source, uint32_t* transform)
{
    void* stateProvider = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);
    void* objectProvider = GetCachedObjectProvider(source, nullptr);
    int stateIndex = GetProviderStateIndex(
        stateProvider, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(objectProvider)));

    if (stateIndex == -1)
        return static_cast<char>(Handle_GetValuePointer(source, nullptr));

    int lookupContext = CSF_AT(
        int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x181C);
    void* stateRegistry = reinterpret_cast<void*>(GetValue8B0554());
    auto* entry = static_cast<C_00833908*>(
        LookupStateEntry(stateRegistry, nullptr, -1, stateIndex, 0, lookupContext));

    if (!entry)
        return 0;

    uint32_t scratch[6] = {};
    void* embeddedObject = reinterpret_cast<uint8_t*>(entry) + 0xC;
    auto initialize = reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        reinterpret_cast<void**>(entry->field_000c)[8 / 4]);
    initialize(embeddedObject, nullptr, scratch);

    char result = SceneLoader_SetObjectTransform(entry, nullptr, transform);
    if (entry->field_0050 == 22)
    {
        entry[1].field_0004 = GetCurrentItemCount(source, nullptr);
        CSF_AT(uint32_t, entry, 0x18C) =
            static_cast<uint32_t>(GetStateAuxiliaryValue(source, nullptr));
        int additionalValue = GetStateAdditionalValue(source, nullptr);
        entry[1].field_000c = static_cast<uint32_t>(additionalValue);
        result = static_cast<char>(additionalValue);
    }
    return result;
}

HOOK(0x004850B0, CharActionFsm_ApplyStateEntryTransform, "auto");
