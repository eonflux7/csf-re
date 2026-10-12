#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuMpSessions_ShutdownChildControls, 0x005600A0, (void* self, void*));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(void*, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

static uint32_t MenuMpSessions_FindControl(uint32_t parentAddress, const char* name)
{
    void* parent = reinterpret_cast<void*>(parentAddress);
    return reinterpret_cast<uint32_t>(
        ((void* (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, parent, 0)[252 / 4])(parent, nullptr, name));
}

static void MenuMpSessions_AddControlOption(uint32_t controlAddress, int option)
{
    void* control = reinterpret_cast<void*>(controlAddress);
    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, control, 0)[668 / 4])(control, nullptr, option);
}

static int __fastcall S_005600a0_InitializeChildControls(
    S_005600a0* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    self->field_0034 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008458E8, "ListboxMapas"));
    if (self->field_0034)
    {
        void* storedObject = ForwardToStoredObjectVirtualSlot40(
            reinterpret_cast<void*>(self->field_0034), nullptr);
        if (storedObject)
        {
            int itemHeight = ((int (__fastcall*)(void*, void*, int))
                CSF_AT(void**, storedObject, 0)[24 / 4])(
                    storedObject, nullptr, 1);
            CGuiListBoxBase_SetItemHeight(
                reinterpret_cast<CGuiListBoxBase*>(self->field_0034),
                nullptr, itemHeight);
        }
    }

    self->field_00ac = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008458DC, "Settings"));
    if (self->field_00ac)
    {
        MenuMpSessions_AddControlOption(self->field_00ac, 1802186800);
        MenuMpSessions_AddControlOption(self->field_00ac, 1802186801);
    }

    self->field_0074 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008458D0, "TipoJuego"));
    if (self->field_0074)
    {
        MenuMpSessions_AddControlOption(self->field_0074, 1668100145);
        MenuMpSessions_AddControlOption(self->field_0074, 1668100146);
        MenuMpSessions_AddControlOption(self->field_0074, 1668100148);
    }

    self->field_0078 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008458C4, "Variable3"));
    if (self->field_0078)
    {
        MenuMpSessions_AddControlOption(self->field_0078, 1668100912);
        MenuMpSessions_AddControlOption(self->field_0078, 1668100913);
    }

    self->field_0024 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008458B4, "SwitchComando"));
    if (self->field_0024)
    {
        MenuMpSessions_AddControlOption(self->field_0024, 1668100912);
        MenuMpSessions_AddControlOption(self->field_0024, 1668100913);
    }

    self->field_0028 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008458A8, "SwitchEspia"));
    if (self->field_0028)
    {
        MenuMpSessions_AddControlOption(self->field_0028, 1668100912);
        MenuMpSessions_AddControlOption(self->field_0028, 1668100913);
    }

    self->field_002c = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845894, "SwitchFrancotirador"));
    if (self->field_002c)
    {
        MenuMpSessions_AddControlOption(self->field_002c, 1668100912);
        MenuMpSessions_AddControlOption(self->field_002c, 1668100913);
    }

    self->field_0030 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845884, "SwitchPrivada"));
    if (self->field_0030)
    {
        MenuMpSessions_AddControlOption(self->field_0030, 1668100912);
        MenuMpSessions_AddControlOption(self->field_0030, 1668100913);
    }

    self->field_007c = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x0084587C, "MaxJug"));
    if (self->field_007c)
    {
        void* control = reinterpret_cast<void*>(self->field_007c);
        ((void (__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, control, 0)[664 / 4])(control, nullptr, 2, 0);

        control = reinterpret_cast<void*>(self->field_007c);
        ((void (__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, control, 0)[668 / 4])(control, nullptr, 16, 0);
    }

    self->field_0080 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845870, "Variable1"));
    self->field_0084 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845864, "Variable2"));
    self->field_00a8 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845854, "StMaxJugTitulo"));
    self->field_0088 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845840, "StTituloVariable1"));
    self->field_008c = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x0084582C, "StTituloVariable2"));
    self->field_0090 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845818, "StTituloVariable3"));
    self->field_0094 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x0084580C, "StComando"));
    self->field_0098 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x00845804, "StEspia"));
    self->field_009c = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008457F4, "StFrancotirador"));
    self->field_00a0 = MenuMpSessions_FindControl(
        self->field_0040, CSF_STR(0x008457E8, "StPrivado"));

    // Packet's vtable entry identifies the direct callee, not a vtable store.
    (void)CSF_VTBL(0x005600A0);
    return MenuMpSessions_ShutdownChildControls(self, nullptr);
}

HOOK(0x005612F0, S_005600a0_InitializeChildControls, "auto");
