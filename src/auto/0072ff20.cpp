#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* object));

// Replaces the "quad"/image pointer stored at CGuiSwitch+0x140 and keeps the
// reference-counted object at +4 alive while it is attached (and copies the
// static control's properties onto the new object).
static void __fastcall CGuiSwitch_SetQuad(
    CGuiSwitch* self, void* /*edx*/, int* quad)
{
    int* previous = static_cast<int*>(CSF_AT(void*, self, 0x140));
    if (quad == previous)
        return;

    if (previous != nullptr) {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(previous) + 4, nullptr);
        CSF_AT(void*, self, 0x140) = nullptr;
    }

    CSF_AT(void*, self, 0x140) = quad;
    if (quad != nullptr) {
        AddObjectReference(
            reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr,
            static_cast<int*>(CSF_AT(void*, self, 0x140)));
    }
}

HOOK(0x0072FF20, CGuiSwitch_SetQuad, "auto");
