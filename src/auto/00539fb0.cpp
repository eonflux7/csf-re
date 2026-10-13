#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DynamicObj_ProcessSubObjectRecord, 0x00443000, (void* self, void* , uint32_t* input));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

static int __fastcall CMenuPause_RunStateOneAction(
    CMenuPause* self, void* /*edx*/)
{
    auto* globalState =
        reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
    const uintptr_t schedulerProxy = reinterpret_cast<uintptr_t>(
        C_00861120_GetSchedulerProxy(globalState, nullptr));

    if (schedulerProxy != 1)
    {
        const int result = static_cast<int>(schedulerProxy - 2);
        if (result == 0)
            self->field_0014 = -1;
        return result;
    }

    SceneLoader_NoOp();
    self->field_0014 = 4;

    alignas(8) uint8_t recordBytes[0x18] = {};
    CSF_AT(int, recordBytes, 0x00) = 0;
    CSF_AT(float, recordBytes, 0x04) = 0.0f;
    CSF_AT(int, recordBytes, 0x08) = 0;

    void* timingState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    CSF_AT(float, recordBytes, 0x04) =
        CSF_AT(float, timingState, 0x2264);

    const int64_t milliseconds = static_cast<int64_t>(
        CSF_AT(float, recordBytes, 0x04) * g_flt_0082E25C);
    CSF_AT(uint32_t, recordBytes, 0x10) =
        static_cast<uint32_t>(milliseconds);
    CSF_AT(int, recordBytes, 0x14) = 0;
    CSF_AT(uint8_t, recordBytes, 0x0C) = 1;

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* subObject = GetGlobalSubObject_4FC0A0(globalObject, nullptr);
    self->field_0044 = DynamicObj_ProcessSubObjectRecord(
        subObject, nullptr, reinterpret_cast<uint32_t*>(recordBytes));

    const int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x20 / 4])(self, nullptr);
    if (result)
    {
        void* sceneEntry = reinterpret_cast<void*>(result);
        return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, sceneEntry, 0)[0xA8 / 4])(
                sceneEntry, nullptr, 1);
    }
    return result;
}

HOOK(0x00539FB0, CMenuPause_RunStateOneAction, "auto");
