#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneSubsystem_LookupEntry, 0x00519AF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

static bool __fastcall C_008344f0_TryPlayAnimation(C_008344f0* self, void* /*edx*/, int a2)
{
    int entryIndex = self->vSlot70(a2);
    if (entryIndex == -1)
        return false;

    int sceneObj = GetValue8B0554();
    void* subsystem = CSF_AT(void*, reinterpret_cast<void*>(sceneObj), 0x1FC4);

    int animEntry = SceneMissionSetup_GetAnimationEntry(subsystem, nullptr, entryIndex);

    int globalInt = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr);
    int accessorResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(accessor), 0)[5])(
        reinterpret_cast<void*>(accessor), nullptr);

    int lookup = SceneSubsystem_LookupEntry(subsystem, nullptr, entryIndex);
    int slot = AccessorState_ResolveSlot(reinterpret_cast<void*>(accessorResult), nullptr, lookup);
    if (slot == 0)
        return false;

    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* target = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(owner, nullptr));

    uint8_t* flags = ((uint8_t * (__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[8])(target, nullptr);
    if ((*flags & 1) == 0) {
        C_00833908_GetTargetName(reinterpret_cast<C_00833908*>(self->field_0008), nullptr);
        return false;
    }

    void* handler = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[26])(target, nullptr));

    alignas(8) uint8_t params[0x24];
    CSF_AT(float, params, 0x00) = static_cast<float>(C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr));
    uint8_t flagByte = CSF_AT(uint8_t, reinterpret_cast<void*>(animEntry), 0x58);
    float scaled = CSF_AT(float, reinterpret_cast<void*>(animEntry), 0x34) * g_SceneLoadMillisecondsToSeconds;
    CSF_AT(uint8_t, params, 0x0C) = flagByte;
    CSF_AT(int, params, 0x08) = 0;
    CSF_AT(uint8_t, params, 0x0D) = 0;
    CSF_AT(uint8_t, params, 0x0E) = 0;
    CSF_AT(int, params, 0x10) = 0x3F800000;
    CSF_AT(int, params, 0x14) = 0x3F800000;
    CSF_AT(int, params, 0x18) = 0x3F800000;
    CSF_AT(uint8_t, params, 0x1C) = 0;
    CSF_AT(uint8_t, params, 0x1D) = 1;
    CSF_AT(float, params, 0x04) = scaled;

    uint8_t result = ((uint8_t(__fastcall*)(void*, void*, int, int, void*))CSF_AT(void**, handler, 0)[5])(
        handler, nullptr, 0, slot, params);
    return result != 0;
}

HOOK(0x004A32F0, C_008344f0_TryPlayAnimation, "auto");
