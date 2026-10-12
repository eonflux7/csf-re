#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, C_00833708_GetActionContext, 0x00489ED0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833708_SetActionState, 0x0048A270, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, CharActionFsm_CheckSubObjectState, 0x00761330, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00833708_DispatchFireAction(
    C_00833708* self, void* /*edx*/, int actionIndex)
{
    auto* actionRecord = reinterpret_cast<uint8_t*>(
        self->field_000c + 96 * static_cast<uint32_t>(actionIndex));

    const int contextAddress =
        C_00833708_GetActionContext(self, nullptr, actionIndex);
    auto* context = reinterpret_cast<void*>(contextAddress);

    if (context)
    {
        if (CSF_AT(uint32_t, context, 0x50))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
                CSF_AT(void**, context, 0)[0x8C / 4])(
                    context, nullptr, 8, 0, -1, 0);
        }
        CSF_AT(uint32_t, actionRecord, 0x54) =
            CSF_AT(uint32_t, context, 0x78) != 1;
    }

    auto* actor = CSF_AT(C_008319c0*, actionRecord, 0);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, actor, 0)[0x40 / 4])(
            actor, nullptr, actionRecord + 0x38);

    C_00833708_SetActionState(self, nullptr, actionIndex, 4);

    int result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (result == contextAddress)
    {
        result = GetGlobalState_50D190();
        auto* listener = CSF_AT(void*, reinterpret_cast<void*>(result), 0x74);
        if (listener)
        {
            const int subObject = C_008319c0_GetSubObjectField74(
                CSF_AT(C_008319c0*, actionRecord, 0), nullptr);
            result = CharActionFsm_CheckSubObjectState(subObject);
            if (static_cast<uint8_t>(result))
            {
                alignas(8) uint8_t eventBytes[0x10];
                std::memset(eventBytes, 0, sizeof(eventBytes));

                const int valueAddress = Handle_GetValuePointer(
                    CSF_AT(C_008319c0*, actionRecord, 0), nullptr);
                CSF_AT(float, eventBytes, 0x0C) = static_cast<float>(
                    CSF_AT(int, reinterpret_cast<void*>(valueAddress), 0x118));

                return reinterpret_cast<int(__fastcall*)(void*, void*, int, void*)>(
                    CSF_AT(void**, listener, 0)[0x4 / 4])(
                        listener, nullptr, 1, eventBytes);
            }
        }
    }
    return result;
}

HOOK(0x00489F50, C_00833708_DispatchFireAction, "auto");
