#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetGlobal8B0660, 0x00519160, ());
static CSF_FUNC(int32_t, __cdecl, GetSharedValue, 0x005191F0, ());
static CSF_FUNC(int, __cdecl, GetDword8B0668, 0x00519270, ());
static CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
static CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
static CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));
static CSF_FUNC(int, __fastcall, MenuSystem_AssignDialogPanel, 0x00712E80, (void* self, void* edx, int panel));

static CSF_GLOBAL(int, g_Value_8B0660, 0x008B0660);
static CSF_GLOBAL(int32_t, g_Value_008B0664, 0x008B0664);
static CSF_GLOBAL(int, g_Value8B0668, 0x008B0668);

static int __fastcall Interface_InitializeDialogPanels(void* self, void* /*edx*/)
{
    void* reference = static_cast<uint8_t*>(self) + 4;
    int panel = g_Value_8B0660;
    if (panel)
    {
        void* object = GetReferencedObject(reference, nullptr);
        CSF_AT(void*, reinterpret_cast<void*>(panel), 0x4) = object;
        GetGlobal8B0660();
        panel = g_Value_8B0660;
        GetGlobal8B0660();
        object = GetReferencedObject(reference, nullptr);
        MenuSystem_AssignDialogPanel(object, nullptr, panel);
    }

    panel = g_Value_008B0664;
    if (panel)
    {
        void* object = GetReferencedObject(reference, nullptr);
        CSF_AT(void*, reinterpret_cast<void*>(panel), 0x4) = object;
        GetSharedValue();
        panel = g_Value_008B0664;
        GetSharedValue();
        object = GetReferencedObject(reference, nullptr);
        MenuSystem_AssignDialogPanel(object, nullptr, panel);
    }

    auto* newPanel = static_cast<C_00830438*>(Mem_Alloc(12));
    if (newPanel)
    {
        C_00830438_Construct(newPanel, nullptr);
        newPanel->vtbl = CSF_VTBL(0x00841F28);
    }

    CSF_AT(C_00830438*, self, 0x15C) = newPanel;
    void* object = GetReferencedObject(reference, nullptr);
    CSF_AT(C_00830438*, self, 0x15C)->field_0004 =
        reinterpret_cast<uint32_t>(object);
    CSF_AT(C_00830438*, self, 0x1C) =
        CSF_AT(C_00830438*, self, 0x15C);

    panel = g_Value8B0668;
    object = GetReferencedObject(reference, nullptr);
    CSF_AT(void*, reinterpret_cast<void*>(panel), 0x4) = object;
    GetDword8B0668();

    panel = reinterpret_cast<int>(CSF_AT(C_00830438*, self, 0x15C));
    object = GetReferencedObject(reference, nullptr);
    MenuSystem_AssignDialogPanel(object, nullptr, panel);

    panel = g_Value8B0668;
    GetDword8B0668();
    object = GetReferencedObject(reference, nullptr);
    return MenuSystem_AssignDialogPanel(object, nullptr, panel);
}

HOOK(0x00536270, Interface_InitializeDialogPanels, "auto");
