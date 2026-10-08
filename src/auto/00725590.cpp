#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::uint32_t, g_ButtonVtable, 0x00865328);

CSF_FUNC(int, __fastcall, ReleaseQuadInterface, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, DestroyEffects, 0x007318F0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* memory));
CSF_FUNC(int, __fastcall, DestroyGuiControl, 0x00719B10,
         (void* self, void* edx));

static int __fastcall CGuiButton_Destroy(CGuiButton* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0) =
        reinterpret_cast<std::uint32_t>(&g_ButtonVtable);

    auto* quadSlot = &self->m_pIdleQuad;
    for (int remaining = 3; remaining != 0; --remaining)
    {
        const std::uint32_t quad = *quadSlot;
        if (quad != 0)
        {
            ReleaseQuadInterface(
                reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
            *quadSlot = 0;
        }
        quadSlot = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(quadSlot) + 0x1C);
    }

    const std::uint32_t permanentQuad = self->m_pPermanentQuad;
    if (permanentQuad != 0)
    {
        ReleaseQuadInterface(
            reinterpret_cast<std::uint8_t*>(permanentQuad) + 4, nullptr);
        self->m_pPermanentQuad = 0;
    }

    void* text = reinterpret_cast<void*>(self->m_pText);
    if (text != nullptr)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, text, 0)[0])(text, nullptr, 1);
        self->m_pText = 0;
    }

    void* effects = reinterpret_cast<void*>(self->m_pFx);
    if (effects != nullptr)
    {
        DestroyEffects(effects, nullptr);
        Mem_Free(effects);
        self->m_pFx = 0;
    }

    return DestroyGuiControl(self, nullptr);
}

HOOK(0x00725590, CGuiButton_Destroy, "auto");
