#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneSubsystem_LookupEntry, 0x00519AF0, (void* self, void* , int entryIndex));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, FindEntryIndex, 0x00771B80, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_NormalizedValueScale, 0x0083BB1C);

static C_0083ba40* __fastcall C_0083ba40_LoadNormalizedValues(
    C_0083ba40* self, void* /*edx*/)
{
    // The packet also identifies the FindEntryIndex address as a vtable.
    (void)CSF_VTBL(0x00771B80);

    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* source = CSF_AT(void*, owner, 0x44);
    void** sourceVtable = CSF_AT(void**, source, 0);

    int sourceIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            sourceVtable[0xFC / 4])(source, nullptr);
    void* handle =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            sourceVtable[0xC8 / 4])(source, nullptr, sourceIndex);

    float firstValue = 0.0f;
    double secondValue = g_fZero;

    if (handle)
    {
        void* scene = CSF_AT(
            void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC4);

        auto* valueData =
            reinterpret_cast<std::uint8_t*>(Handle_GetValuePointer(handle, nullptr));
        int entryIndex = FindEntryIndex(valueData + 0x184, nullptr, 14);
        int entryId = -1;
        if (entryIndex != -1)
        {
            valueData =
                reinterpret_cast<std::uint8_t*>(Handle_GetValuePointer(handle, nullptr));
            void* entries = CSF_AT(void*, valueData, 0x188);
            entryId = CSF_AT(int, entries, 12 * entryIndex);
        }

        int sceneIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, scene, 0)[0x8 / 4])(scene, nullptr, entryId);
        if (sceneIndex != -1)
        {
            int entry = SceneSubsystem_LookupEntry(scene, nullptr, sceneIndex);
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* accessor =
                reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
            void* state =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);
            int slot = AccessorState_ResolveSlot(state, nullptr, entry);
            if (slot)
                firstValue = static_cast<float>(
                    C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr));
            else
                firstValue = 0.0f;
        }

        valueData =
            reinterpret_cast<std::uint8_t*>(Handle_GetValuePointer(handle, nullptr));
        entryIndex = FindEntryIndex(valueData + 0x184, nullptr, 15);
        entryId = -1;
        if (entryIndex != -1)
        {
            valueData =
                reinterpret_cast<std::uint8_t*>(Handle_GetValuePointer(handle, nullptr));
            void* entries = CSF_AT(void*, valueData, 0x188);
            entryId = CSF_AT(int, entries, 12 * entryIndex);
        }

        sceneIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, scene, 0)[0x8 / 4])(scene, nullptr, entryId);
        if (sceneIndex != -1)
        {
            int entry = SceneSubsystem_LookupEntry(scene, nullptr, sceneIndex);
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* accessor =
                reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
            void* state =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);
            int slot = AccessorState_ResolveSlot(state, nullptr, entry);
            if (slot)
                secondValue =
                    C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr);
            else
                secondValue = g_fZero;
        }
    }

    float normalizedFirst = firstValue * g_NormalizedValueScale;
    std::memcpy(&self->field_0070, &normalizedFirst, sizeof(normalizedFirst));
    float normalizedSecond =
        static_cast<float>(secondValue * g_NormalizedValueScale);
    std::memcpy(&self->field_0074, &normalizedSecond, sizeof(normalizedSecond));
    return self;
}

HOOK(0x004D31E0, C_0083ba40_LoadNormalizedValues, "auto");
