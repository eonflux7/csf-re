#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __cdecl CMenuMessageBox_BindAndInitializeControl(
    CMenuMessageBox* self, std::uint32_t& destination, const char* name)
{
    void* root = reinterpret_cast<void*>(self->field_0894);
    void* control =
        ((void* (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, root, 0)[252 / 4])(root, nullptr, name);

    destination = reinterpret_cast<std::uint32_t>(control);
    if (control)
    {
        return ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[168 / 4])(control, nullptr, 1);
    }
    return 0;
}

static int __fastcall CMenuMessageBox_InitializeControls(
    CMenuMessageBox* self, void* /*edx*/)
{
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_002c, CSF_STR(0x008412F4, "BotonOK"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0030, CSF_STR(0x008412E8, "BotonCancel"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0034, CSF_STR(0x008412DC, "BotonYes"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0038, CSF_STR(0x008412D4, "BotonNo"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_003c, CSF_STR(0x008412C8, "BotonRetry"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0040, CSF_STR(0x008412B8, "BotonContinue"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0044, CSF_STR(0x008412AC, "BotonSlot1"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0048, CSF_STR(0x008412A0, "BotonSlot2"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_004c, CSF_STR(0x00841294, "BotonSlot3"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0050, CSF_STR(0x00841288, "BotonSlot4"));
    CMenuMessageBox_BindAndInitializeControl(
        self, self->field_0054, CSF_STR(0x0084127C, "BotonSlot5"));
    return CMenuMessageBox_BindAndInitializeControl(
        self, self->m_staticControl, CSF_STR(0x00841274, "Static"));
}

HOOK(0x005290B0, CMenuMessageBox_InitializeControls, "auto");
