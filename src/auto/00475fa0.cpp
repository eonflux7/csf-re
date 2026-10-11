#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_444b10_IncrementCounter_0x50, 0x00444B10, (void* self, void* , int));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CScene_AddObjectFlags, 0x00452800, (CScene* self, void* , int handleAddress, int flags));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008316d8_ProcessAction(
    C_008316d8* self, void* /*edx*/, int handle)
{
    const int remaining = static_cast<int32_t>(self->field_001c);
    if (remaining <= 0)
        return remaining;

    self->vSlotB0();

    int actionValue = CSF_AT(
        int, reinterpret_cast<void*>(self->field_0040), 0x118);
    const int handleIndex = CSF_AT(int, reinterpret_cast<void*>(handle), 0x8);
    uint32_t objectAddress = self->field_0004;

    if (handleIndex >= 0)
    {
        const int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        if (resolved != 0)
        {
            objectAddress = static_cast<uint32_t>(resolved);
            const int globalControl = GetGlobalValue_8B0568();
            const int colorAnim = CGuiControl_GetColorAnim(
                reinterpret_cast<CGuiControl*>(globalControl), nullptr);
            C_444b10_IncrementCounter_0x50(
                reinterpret_cast<void*>(colorAnim), nullptr, resolved);
        }
    }

    if (CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x118) > 0 &&
        static_cast<uint8_t>(SceneObj_IsInactive(
            reinterpret_cast<void*>(objectAddress), nullptr)) != 0)
    {
        CScene_AddObjectFlags(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            static_cast<int>(objectAddress + 0x1C), 1);
    }

    void* actionObject = CSF_AT(
        void*, reinterpret_cast<void*>(objectAddress), 0x38);
    ((void(__fastcall*)(void*, void*, int*))
        CSF_AT(void**, actionObject, 0)[0x98 / 4])(
            actionObject, nullptr, &actionValue);

    if (self->vSlot9C())
        --self->field_001c;

    return self->vSlot80();
}

HOOK(0x00475FA0, C_008316d8_ProcessAction, "auto");
