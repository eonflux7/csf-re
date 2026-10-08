#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// 0x006923B0 : __thiscall, takes ref-counted object at (this - 4) ... adds a reference
CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0,
         (void* self, void* edx));
// 0x006923C0 : __thiscall, releases the reference
CSF_FUNC(void, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
// 0x007181E0 : __thiscall, copies shared GUI properties onto the given object
CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* object));

// Sets the switch's associated (ref-counted) quad/object stored at +0x13C,
// releasing the previous one and copying the shared properties to the new one.
static void __fastcall CGuiSwitch_SetQuad(CGuiSwitch* self, void* /*edx*/, int* quad)
{
    auto* previous = reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x13C));
    if (quad == previous)
        return;

    if (previous) {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(previous) + 4, nullptr);
        CSF_AT(std::uint32_t, self, 0x13C) = 0;
    }

    CSF_AT(std::uint32_t, self, 0x13C) = reinterpret_cast<std::uint32_t>(quad);
    if (quad) {
        AddObjectReference(
            reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr,
            reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x13C)));
    }
}

HOOK(0x0072FED0, CGuiSwitch_SetQuad, "auto");
