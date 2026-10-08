#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* object));

// Sets the "pressed" quad of a CGuiButton, releasing the previous quad and
// copying the new quad's properties onto the button.
static void __fastcall CGuiButton_SetPressedQuad(
    CGuiButton* self, void* /*edx*/, int* quad)
{
    auto* previous = reinterpret_cast<int*>(self->m_pPressedQuad);
    if (quad == previous)
        return;

    if (previous) {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(previous) + 4, nullptr);
        self->m_pPressedQuad = 0;
    }

    self->m_pPressedQuad = reinterpret_cast<std::uint32_t>(quad);
    if (quad) {
        AddObjectReference(
            reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
        CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->m_pPressedQuad));
    }
}

HOOK(0x00724E00, CGuiButton_SetPressedQuad, "auto");
