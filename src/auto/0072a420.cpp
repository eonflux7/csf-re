#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, AddReference, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseReference, 0x006923C0,
         (void* self, void* edx));

static void __fastcall CGuiPanel_SetPointerQuad(
    CGuiPanel* self, void* /*edx*/, std::uint32_t pointerQuad)
{
    const std::uint32_t previousQuad = self->m_pPointerQuad;
    if (pointerQuad == previousQuad)
        return;

    if (previousQuad != 0) {
        ReleaseReference(
            reinterpret_cast<std::uint8_t*>(previousQuad) + 4, nullptr);
        self->m_pPointerQuad = 0;
    }

    self->m_pPointerQuad = pointerQuad;
    if (pointerQuad != 0) {
        AddReference(
            reinterpret_cast<std::uint8_t*>(pointerQuad) + 4, nullptr);
    }
}

HOOK(0x0072A420, CGuiPanel_SetPointerQuad, "auto");
