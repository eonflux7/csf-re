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

static void __fastcall CGuiButton_SetHoverQuad(
    CGuiButton* self, void* /*edx*/, int* quad)
{
    auto* previous = reinterpret_cast<int*>(self->m_pHoverQuad);
    if (quad == previous)
        return;

    if (previous) {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(previous) + 4, nullptr);
        self->m_pHoverQuad = 0;
    }

    self->m_pHoverQuad = reinterpret_cast<std::uint32_t>(quad);
    if (quad) {
        AddObjectReference(
            reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
        CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->m_pHoverQuad));
    }
}

HOOK(0x00724DB0, CGuiButton_SetHoverQuad, "auto");

