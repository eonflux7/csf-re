#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::uint32_t, g_SwitchVtable, 0x00866B58);

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_Destroy, 0x00719B10,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiSwitch_Destroy(CGuiSwitch* self, void* /*edx*/)
{
    void* resource = CSF_AT(void*, self, 0x134);
    CSF_AT(void**, self, 0) = reinterpret_cast<void**>(&g_SwitchVtable);
    if (resource)
    {
        ReleaseObjectReference(static_cast<std::uint8_t*>(resource) + 4, nullptr);
        CSF_AT(void*, self, 0x134) = nullptr;
    }

    resource = CSF_AT(void*, self, 0x138);
    if (resource)
    {
        ReleaseObjectReference(static_cast<std::uint8_t*>(resource) + 4, nullptr);
        CSF_AT(void*, self, 0x138) = nullptr;
    }

    resource = CSF_AT(void*, self, 0x13C);
    if (resource)
    {
        ReleaseObjectReference(static_cast<std::uint8_t*>(resource) + 4, nullptr);
        CSF_AT(void*, self, 0x13C) = nullptr;
    }

    resource = CSF_AT(void*, self, 0x140);
    if (resource)
    {
        ReleaseObjectReference(static_cast<std::uint8_t*>(resource) + 4, nullptr);
        CSF_AT(void*, self, 0x140) = nullptr;
    }

    void* ownedObject = CSF_AT(void*, self, 0x148);
    if (ownedObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        CSF_AT(void*, self, 0x148) = nullptr;
    }

    return CGuiControl_Destroy(self, nullptr);
}

HOOK(0x0072FCF0, CGuiSwitch_Destroy, "auto");
