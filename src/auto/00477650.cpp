#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, Tables_ResolveAnimationIndex, 0x00519B50, (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __fastcall, BddTables_GetEntryElementCount, 0x00519BE0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* , int a2));
CSF_FUNC(int, __fastcall, SoundAreas_FindEntryIndex, 0x00771B80, (S_00771b80* self, void* , int searchValue));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* , int entryIndex));

CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

static unsigned int __fastcall C_008316d8_PlayAnimationEntry(
    C_008316d8* self, void* /*edx*/, int entryKey,
    unsigned int elementIndex, float startTime)
{
    unsigned int result = self->vResolveEntryLookupValue();
    if (!result)
        return result;

    void* animationOwner = reinterpret_cast<void*>(result);
    result = reinterpret_cast<unsigned int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, animationOwner, 0)[0x68 / 4])(animationOwner, nullptr);
    if (!result)
        return result;

    (void)CSF_VTBL(0x00771B80);
    int entryIndex = SoundAreas_FindEntryIndex(
        reinterpret_cast<S_00771b80*>(
            reinterpret_cast<uint8_t*>(self->field_0040) + 0x154),
        nullptr, entryKey);

    if (entryIndex == -1)
    {
        void* controller = reinterpret_cast<void*>(
            reinterpret_cast<unsigned int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, animationOwner, 0)[0x68 / 4])(
                    animationOwner, nullptr));
        result = reinterpret_cast<unsigned int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr, 0);
        if (!result)
            return result;

        controller = reinterpret_cast<void*>(
            reinterpret_cast<unsigned int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, animationOwner, 0)[0x68 / 4])(
                    animationOwner, nullptr));
        return reinterpret_cast<unsigned int(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, controller, 0)[0x1C / 4])(controller, nullptr, 0, 0);
    }

    void* entries = CSF_AT(void*, reinterpret_cast<void*>(self->field_0040), 0x158);
    int tableIndex = CSF_AT(
        int, reinterpret_cast<uint8_t*>(entries) + 12 * entryIndex, 8);

    void* setup = reinterpret_cast<void*>(GetValue8B0554());
    void* tables = CSF_AT(void*, setup, 0x1FC4);
    void* animationEntry = reinterpret_cast<void*>(
        SceneMissionSetup_GetAnimationEntry(tables, nullptr, tableIndex));

    if (elementIndex != static_cast<unsigned int>(-1))
    {
        result = BddTables_GetEntryElementCount(tables, nullptr, tableIndex);
        if (elementIndex >= result)
            return result;
    }

    void* globalInt = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalInt, nullptr));

    // The earlier push supplies this call's single stack argument.
    // Its incoming-register value is unspecified, so use zero.
    void* accessorState = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr, 0);

    int animationIndex = Tables_ResolveAnimationIndex(
        tables, nullptr, tableIndex, &elementIndex);
    self->field_0044 = animationIndex;
    void* animationSlot = reinterpret_cast<void*>(
        AccessorState_ResolveSlot(accessorState, nullptr, animationIndex));

    alignas(8) uint8_t parameters[0x28] = {};
    CSF_AT(float, parameters, 0x00) = 0.0f;
    CSF_AT(float, parameters, 0x04) =
        CSF_AT(float, animationEntry, 0x34) * g_SceneLoadMillisecondsToSeconds;
    CSF_AT(float, parameters, 0x08) = 0.0f;
    CSF_AT(uint8_t, parameters, 0x0C) = CSF_AT(uint8_t, animationEntry, 0x58);
    CSF_AT(uint8_t, parameters, 0x0D) = 0;
    CSF_AT(uint8_t, parameters, 0x0E) = 0;
    CSF_AT(float, parameters, 0x10) = 1.0f;
    CSF_AT(float, parameters, 0x14) = 1.0f;
    CSF_AT(float, parameters, 0x18) = 1.0f;
    CSF_AT(uint8_t, parameters, 0x1C) = 0;
    CSF_AT(uint8_t, parameters, 0x1D) = 1;
    CSF_AT(void*, parameters, 0x20) = animationOwner;
    CSF_AT(void*, parameters, 0x24) = animationEntry;

    if (C_Unknown_GetValue(animationSlot, nullptr) < startTime)
        CSF_AT(float, parameters, 0x00) =
            static_cast<float>(C_Unknown_GetValue(animationSlot, nullptr));
    else
        CSF_AT(float, parameters, 0x00) = startTime;

    void* controller = reinterpret_cast<void*>(
        reinterpret_cast<unsigned int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, animationOwner, 0)[0x68 / 4])(animationOwner, nullptr));
    return reinterpret_cast<unsigned int(__fastcall*)(void*, void*, int, void*, void*)>(
        CSF_AT(void**, controller, 0)[0x14 / 4])(
            controller, nullptr, 0, animationSlot, parameters);
}

HOOK(0x00477650, C_008316d8_PlayAnimationEntry, "auto");
