#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuMessageBox_ResetControlsAndReleaseRaster, 0x00528980, (CMenuMessageBox* self, void*));
CSF_FUNC(int, __fastcall, CMenuMessageBox_CenterOnTarget, 0x00528F40, (CMenuMessageBox* self, void*));
CSF_FUNC(int, __fastcall, CMenuMessageBox_InitializeControls, 0x005290B0, (CMenuMessageBox* self, void*));
CSF_FUNC(void, __fastcall, CMenuMessageBox_ForwardToChildren, 0x0052A7F0, (CMenuMessageBox* self, void* , int value));

static int __fastcall CMenuMessageBox_ShowOnTarget(
    CMenuMessageBox* self, void* /*edx*/, int target, int /*unused*/)
{
    CMenuMessageBox_ResetControlsAndReleaseRaster(self, nullptr);
    self->field_0890 = target;
    CMenuMessageBox_InitializeControls(self, nullptr);

    void* control = reinterpret_cast<void*>(self->field_005c);
    if (control)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

    control = reinterpret_cast<void*>(self->field_0060);
    if (control)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

    control = reinterpret_cast<void*>(self->field_0064);
    if (control)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

    control = reinterpret_cast<void*>(self->field_0068);
    if (control)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

    control = reinterpret_cast<void*>(self->field_006c);
    if (control)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

    control = reinterpret_cast<void*>(self->field_0070);
    if (control)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

    control = reinterpret_cast<void*>(self->field_0074);
    if (control)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, 1);

    self->vDispatchByStateFlag();

    if (self->field_08ac)
    {
        CMenuMessageBox_ForwardToChildren(self, nullptr, 0);

        const uint32_t state = self->field_08a8;
        void* dialog = reinterpret_cast<void*>(self->field_0894);
        // The original replaces only the low byte of the state argument.
        const uint32_t argument = (state & ~0xFFu) | (state == 0 ? 1u : 0u);
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, dialog, 0)[0x25C / 4])(
            dialog, nullptr, argument);

        dialog = reinterpret_cast<void*>(self->field_0894);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, dialog, 0)[0x16C / 4])(
            dialog, nullptr);
    }
    else
    {
        CMenuMessageBox_ForwardToChildren(self, nullptr, 1);
    }

    void* dialog = reinterpret_cast<void*>(self->field_0894);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, dialog, 0)[0xC0 / 4])(
        dialog, nullptr, self->field_0948);

    CMenuMessageBox_CenterOnTarget(self, nullptr);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    C_00830578* controller = CSF_AT(C_00830578*, globalState, 0x18);
    void* targetObject = reinterpret_cast<void*>(self->field_0890);
    if (((bool(__fastcall*)(void*, void*, C_00830578*, int))
             CSF_AT(void**, targetObject, 0)[0x74 / 4])(
            targetObject, nullptr, controller, 0))
    {
        globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        C_00830578_ResetMenuController(
            CSF_AT(C_00830578*, globalState, 0x18), nullptr);
    }

    return static_cast<int>(self->field_0894);
}

HOOK(0x0052AD20, CMenuMessageBox_ShowOnTarget, "auto");
