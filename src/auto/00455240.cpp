#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(void*, __fastcall, SceneLoader_PrepareObjectResources, 0x00444130, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareEnvironment, 0x00444870, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_PrepareSceneObjects, 0x004458F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_ResetSceneState, 0x004480C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneData, 0x0044D360, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ResetPreparation, 0x0044F300, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_ReservePrimaryEntries, 0x004549E0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_ReserveSecondaryEntries, 0x00454AF0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneActors, 0x0046B170, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_SetResolvedObject, 0x004A0880, (C_008343a4* self, void* edx, uint32_t object));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_PrepareGlobalSceneState, 0x004E8C90, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static char __fastcall S_0044f300_PrepareCurrentScene(
    S_0044f300* self, void* /*edx*/)
{
    if (self->field_000c)
        return static_cast<char>(self->field_000c);

    SceneLoader_ResetPreparation(self, nullptr);

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    if (self->field_0014 < 0xC8)
        GrowButtonArray(bytes + 0x10, nullptr, 200 - self->field_0014);

    for (uint32_t index = CSF_AT(uint32_t, self, 0x18);
         index < 0xC8; ++index)
    {
        reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x10))[index] = 0;
    }
    CSF_AT(uint32_t, self, 0x18) = 200;

    if (self->field_0024 < 0xC8)
        GrowButtonArray(bytes + 0x20, nullptr, 200 - self->field_0024);
    if (self->field_0034 < 0xA)
        GrowButtonArray(bytes + 0x30, nullptr, 10 - self->field_0034);
    if (self->field_0044 < 0xA)
        GrowButtonArray(bytes + 0x40, nullptr, 10 - self->field_0044);
    if (self->field_0064 < 0xA)
        GrowButtonArray(bytes + 0x60, nullptr, 10 - self->field_0064);
    if (self->field_009c < 0xA)
        GrowButtonArray(bytes + 0x98, nullptr, 10 - self->field_009c);

    SceneLoader_ReservePrimaryEntries(bytes + 0xFC, nullptr, 0xAu);
    SceneLoader_ReserveSecondaryEntries(bytes + 0x10C, nullptr, 10);

    if (self->field_0054 < 0xC8)
        GrowButtonArray(bytes + 0x50, nullptr, 200 - self->field_0054);

    if (self->field_012c)
        SceneLoader_NoOp();

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* resolvedObject = MenuControls_ResolveObject(globalObject, nullptr);
    SceneLoader_SetResolvedObject(
        reinterpret_cast<C_008343a4*>(self->field_00ac), nullptr,
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(resolvedObject)));

    int globalSceneState = GetValue8B0554();
    SceneLoader_PrepareGlobalSceneState(
        reinterpret_cast<void*>(globalSceneState), nullptr);
    SceneLoader_PrepareSceneActors(
        reinterpret_cast<void*>(self->field_00bc), nullptr);
    SceneLoader_PrepareEnvironment(
        reinterpret_cast<void*>(self->field_00c0), nullptr);
    SceneLoader_PrepareSceneObjects(
        reinterpret_cast<void*>(self->field_00b0), nullptr);
    SceneLoader_PrepareSceneData(
        reinterpret_cast<void*>(self->field_00b4), nullptr);
    SceneLoader_PrepareObjectResources(
        reinterpret_cast<void*>(self->field_00b8), nullptr);
    SceneLoader_ResetSceneState(
        reinterpret_cast<void*>(self->field_00c4), nullptr);
    SceneLoader_NoOp();

    self->field_000c = 1;
    // The first-time path leaves only an incidental register value in AL.
    return 0;
}

HOOK(0x00455240, S_0044f300_PrepareCurrentScene, "auto");
