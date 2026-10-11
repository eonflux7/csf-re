#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(void, __fastcall, UpdateAuxiliaryController, 0x00443760, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, UpdateSecondaryController, 0x00444930, (void* self, void* edx, void* a1));
CSF_FUNC(int, __fastcall, UpdatePrimaryController, 0x00447940, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, UpdateFinalController, 0x00448DA0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, S_00452280_UpdateTimers, 0x00452280, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, S_00452280_UpdateControls, 0x004523C0, (void* self, void* edx, int _AC));
CSF_FUNC(uint32_t, __fastcall, S_00452280_RefreshState, 0x004539B0, (CScene* self, void* edx));
CSF_FUNC(int, __fastcall, S_00452280_UpdateSceneState, 0x00453A80, (void* self, void* edx, int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_457A80, 0x00457A80, ());
CSF_FUNC(char, __fastcall, UpdateGlobalController, 0x004580C0, (void* self, void* edx, float a2));
CSF_FUNC(unsigned int, __fastcall, UpdateFrameController, 0x00469800, (void* self, void* edx, float a2));
CSF_FUNC(unsigned int, __fastcall, RefreshEventDispatcher, 0x00475630, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(void, __fastcall, UpdateTimeController, 0x0048AA40, (void* self, void* edx, float a2));
CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, UpdateManagedController, 0x004EF780, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetProviderFrameState, 0x004F4EA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, RefreshGlobalFrameState, 0x00521DA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, RefreshGlobalTransformState, 0x00521DB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static void __fastcall S_00452280_Update(
    S_00452280* self, void* /*edx*/, float deltaTime)
{
    // These two entries in the packet also identify direct-call targets.
    (void)CSF_VTBL(0x004EF780);
    (void)CSF_VTBL(0x00452280);

    // The transform result overlaps the original frame's saved timestep.
    std::uint32_t transformResult[4] = {};
    std::memcpy(&transformResult[2], &deltaTime, sizeof(deltaTime));

    if (self->field_0128)
        deltaTime = 0.0f;

    std::uint32_t deltaBits;
    std::memcpy(&deltaBits, &deltaTime, sizeof(deltaBits));

    UpdateTimeController(
        reinterpret_cast<void*>(self->field_00a8), nullptr, deltaTime);
    self->field_0084 = 0;
    self->field_0008 += deltaTime;
    self->field_011c -= deltaTime;

    if (self->field_011c < g_fZero)
    {
        alignas(4) std::uint8_t record[0x34];
        CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(std::uint32_t, record, 0x04) = 24;
        CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(int, record, 0x0C) = -1;
        CSF_AT(int, record, 0x10) = -1;
        CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(int, record, 0x18) = -1;
        CSF_AT(int, record, 0x1C) = -1;
        std::memset(record + 0x20, 0, 16);
        CSF_AT(std::uint8_t, record, 0x30) = 0;

        DispatchRecordToGlobalObject(
            reinterpret_cast<void*>(self->field_0184), nullptr, record, 1);
        self->field_011c = CSF_AT(
            float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA88);
    }

    RefreshEventDispatcher(reinterpret_cast<void*>(self->field_0184), nullptr);
    UpdateGlobalController(
        reinterpret_cast<void*>(GetGlobalValue_457A80()), nullptr, deltaTime);
    UpdateManagedController(
        CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC),
        nullptr, static_cast<int>(deltaBits));

    if (self->field_00b0)
        UpdatePrimaryController(reinterpret_cast<void*>(self->field_00b0),
                                nullptr, reinterpret_cast<void*>(deltaBits));
    if (self->field_00b4)
        C_0085ec18_NullSub(static_cast<int>(deltaBits));
    if (self->field_00b8)
        UpdateAuxiliaryController(
            reinterpret_cast<void*>(self->field_00b8), nullptr, deltaTime);
    if (self->field_00bc)
        UpdateFrameController(
            reinterpret_cast<void*>(self->field_00bc), nullptr, deltaTime);
    if (self->field_00c0)
        UpdateSecondaryController(reinterpret_cast<void*>(self->field_00c0),
                                  nullptr, reinterpret_cast<void*>(deltaBits));
    if (self->field_00c4)
        UpdateFinalController(
            reinterpret_cast<void*>(self->field_00c4), nullptr, deltaTime);

    void* first = reinterpret_cast<void*>(GetFirstElement(
        reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr));
    void* provider = GetCachedObjectProvider(first, nullptr);
    // Both forwarders tail-jump to a virtual of the global object: this in ecx, one argument.
    const int frameState = GetProviderFrameState(provider, nullptr);
    RefreshGlobalFrameState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr, frameState);

    void* referenced = GetReferencedObject(provider, nullptr);
    void* transformSource =
        ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);
    const int transform = ((int(__fastcall*)(void*, void*, void*))
         CSF_AT(void**, transformSource, 0)[0x8C / 4])(
             transformSource, nullptr, transformResult);

    RefreshGlobalTransformState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr, transform);
    if (static_cast<std::uint8_t>(transformResult[1]))
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(transformResult[0]));

    void* selectedObject = nullptr;
    const std::uint32_t selectedIndex = self->field_0070;
    if (selectedIndex != static_cast<std::uint32_t>(-1) &&
        selectedIndex < self->field_0068)
    {
        selectedObject = reinterpret_cast<void*>(
            reinterpret_cast<std::uint32_t*>(self->field_0060)[selectedIndex]);
    }

    const std::uint32_t objectCount = self->field_0018;
    for (std::uint32_t index = 0; index < objectCount; ++index)
    {
        void* object = reinterpret_cast<void*>(
            reinterpret_cast<std::uint32_t*>(self->field_0010)[index]);
        if (!object || !(CSF_AT(std::uint8_t, object, 0x170) & 1))
            continue;

        std::uint32_t objectDelta;
        if (object == selectedObject ||
            static_cast<std::uint8_t>(
                Scene_IsObjectRemovable(CSF_AT(int, object, 0x50))))
        {
            objectDelta = transformResult[2];
        }
        else
        {
            if (!(deltaTime > g_fZero))
                continue;
            objectDelta = deltaBits;
        }

        ((void(__fastcall*)(void*, void*, std::uint32_t))
             CSF_AT(void**, object, 0)[0x20 / 4])(
                 object, nullptr, objectDelta);
    }

    S_00452280_RefreshState(reinterpret_cast<CScene*>(self), nullptr);
    S_00452280_UpdateSceneState(self, nullptr, static_cast<int>(deltaBits));
    S_00452280_UpdateTimers(self, nullptr, deltaTime);
    S_00452280_UpdateControls(self, nullptr, static_cast<int>(deltaBits));
}

HOOK(0x00454070, S_00452280_Update, "auto");

