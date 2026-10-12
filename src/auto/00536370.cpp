#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0660, 0x00519160, ());
CSF_FUNC(int32_t, __cdecl, GetSharedValue, 0x005191F0, ());
CSF_FUNC(int, __cdecl, GetDword8B0668, 0x00519270, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MenuControls_ApplyChildIndex, 0x00712D90, (void* self, void* edx, int index));

CSF_GLOBAL(int, g_Value_8B0660, 0x008B0660);
CSF_GLOBAL(int32_t, g_Value_008B0664, 0x008B0664);
CSF_GLOBAL(int, g_Value8B0668, 0x008B0668);

static void __fastcall Interface_ClearMenuControls(void* self, void* /*edx*/)
{
    int childIndex = g_Value8B0668;
    GetDword8B0668();
    if (childIndex)
    {
        childIndex = g_Value8B0668;
        GetDword8B0668();
        void* controls = GetReferencedObject(
            static_cast<uint8_t*>(self) + 4, nullptr);
        MenuControls_ApplyChildIndex(controls, nullptr, childIndex);
    }

    childIndex = g_Value_008B0664;
    if (childIndex)
    {
        GetSharedValue();
        childIndex = g_Value_008B0664;
        GetSharedValue();
        void* controls = GetReferencedObject(
            static_cast<uint8_t*>(self) + 4, nullptr);
        MenuControls_ApplyChildIndex(controls, nullptr, childIndex);
    }

    childIndex = g_Value_8B0660;
    if (childIndex)
    {
        GetGlobal8B0660();
        childIndex = g_Value_8B0660;
        GetGlobal8B0660();
        void* controls = GetReferencedObject(
            static_cast<uint8_t*>(self) + 4, nullptr);
        MenuControls_ApplyChildIndex(controls, nullptr, childIndex);
    }

    childIndex = CSF_AT(int, self, 0x15C);
    if (childIndex)
    {
        void* controls = GetReferencedObject(
            static_cast<uint8_t*>(self) + 4, nullptr);
        MenuControls_ApplyChildIndex(controls, nullptr, childIndex);
    }

    void* ownedObject = CSF_AT(void*, self, 0x15C);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        CSF_AT(int, self, 0x15C) = 0;
    }
}

HOOK(0x00536370, Interface_ClearMenuControls, "auto");
