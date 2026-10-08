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

// Sets the reference-counted object attached to this static control's +0x138
// slot, releasing any previous one and copying this control's properties into
// the new one.
static void __fastcall CGuiStatic_SetAttachedObject(
    CGuiStatic* self, void* /*edx*/, int* object)
{
    auto* previous = reinterpret_cast<int*>(self->field_0138);
    if (object == previous)
        return;

    if (previous) {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(previous) + 4, nullptr);
        self->field_0138 = 0;
    }

    self->field_0138 = reinterpret_cast<std::uint32_t>(object);
    if (object) {
        AddObjectReference(
            reinterpret_cast<std::uint8_t*>(object) + 4, nullptr);
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->field_0138));
    }
}

HOOK(0x0072FE30, CGuiStatic_SetAttachedObject, "auto");
