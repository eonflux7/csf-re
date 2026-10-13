#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00863430_GetTargetTransform, 0x00409470, (C_00863430* self, void* , uint32_t* outBuffer));
CSF_FUNC(int, __cdecl, AiTaskStates_IsDistanceWithin200, 0x0041E350, (S_004e0c20* a1, S_004e0c20* a2));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(unsigned int, __fastcall, C_0083bc28_SetSkinDirectionAndFlags, 0x004D3830, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083bc28_ResetSkinDirectionAndFlags, 0x004D3870, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* , unsigned int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_0083bc28_ProcessPendingTargetUpdates(
    C_0083bc28* self, void* /*edx*/)
{
    if ((self->field_0044 & 0x101) == 0)
        return;

    if (!self->field_0054)
    {
        alignas(8) uint8_t transformBytes[0xC];
        auto* transform = reinterpret_cast<S_004e0c20*>(transformBytes);

        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* skinTarget = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x4C));
        auto* targetObject =
            ((C_00863430* (__fastcall*)(void*, void*))
                CSF_AT(void**, skinTarget, 0)[0x3C / 4])(skinTarget, nullptr);

        C_00863430_GetTargetTransform(
            targetObject, nullptr, reinterpret_cast<uint32_t*>(transformBytes));

        uint32_t sceneObjects = CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
        int foundObject = SceneFindSubObject(
            reinterpret_cast<void*>(sceneObjects), nullptr,
            reinterpret_cast<uintptr_t>(transform));

        self->field_0054 = static_cast<uint32_t>(foundObject);
        if (foundObject &&
            static_cast<uint8_t>(AiTaskStates_IsDistanceWithin200(
                reinterpret_cast<S_004e0c20*>(foundObject), transform)) != 0)
        {
            void* object = reinterpret_cast<void*>(self->field_0054);
            self->field_0050 = CSF_AT(uint32_t, object, 0);
            CSF_AT(uint32_t, object, 4) |= 2u;
        }
        else
        {
            self->field_0054 = 0;
        }
    }

    if ((self->field_0044 & 1) != 0)
    {
        if ((CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0014), 0x98) & 1) != 0)
            C_0083bc28_SetSkinDirectionAndFlags(self, nullptr);
        else
            C_0083bc28_ResetSkinDirectionAndFlags(self, nullptr);

        if ((CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0014), 0x98) & 2) != 0)
            self->field_0044 |= 4u;
    }

    self->field_0044 &= 0xFFFFFEFE;
}

HOOK(0x4D3B30, C_0083bc28_ProcessPendingTargetUpdates, "auto");
