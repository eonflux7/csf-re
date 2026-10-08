#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* object));

static void __fastcall CGuiButton_SetIdleQuad(
    CGuiButton* self, void* /*edx*/, int* quad)
{
    auto* previous = reinterpret_cast<int*>(self->m_pIdleQuad);
    if (quad == previous)
        return;

    if (previous) {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(previous) + 4, nullptr);
        self->m_pIdleQuad = 0;
    }

    self->m_pIdleQuad = reinterpret_cast<std::uint32_t>(quad);
    if (quad) {
        AddObjectReference(
            reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
        CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->m_pIdleQuad));
    }
}

HOOK(0x00724D60, CGuiButton_SetIdleQuad, "auto");
