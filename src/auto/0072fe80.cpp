#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Reference-counting helpers on a sub-object embedded at +4 inside a refcounted object.
CSF_FUNC(void, __fastcall, RetainObjectRef, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseObjectRef, 0x006923C0,
         (void* self, void* edx));

// Copies the gui control properties onto the given target object.
CSF_FUNC(void, __fastcall, CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* object));

// CGuiSwitch::SetLinkedObject - assigns the reference-counted object stored at +0x134.
static void __fastcall CGuiSwitch_SetLinkedObject(
    CGuiSwitch* self, void* /*edx*/, int* object)
{
    auto* previous = reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x134));
    if (object == previous)
        return;

    if (previous) {
        ReleaseObjectRef(
            reinterpret_cast<std::uint8_t*>(previous) + 4, nullptr);
        CSF_AT(std::uint32_t, self, 0x134) = 0;
    }

    CSF_AT(std::uint32_t, self, 0x134) = reinterpret_cast<std::uint32_t>(object);
    if (object) {
        RetainObjectRef(
            reinterpret_cast<std::uint8_t*>(object) + 4, nullptr);
        CopyPropertiesToObject(
            self, nullptr,
            reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x134)));
    }
}

HOOK(0x0072FE80, CGuiSwitch_SetLinkedObject, "auto");
