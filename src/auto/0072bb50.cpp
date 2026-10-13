#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"


CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_Destroy, 0x00719B10,
         (C_00830438* self, void* edx));

static int __fastcall CGuiPanel_Destroy(CGuiPanel* self, void* /*edx*/)
{
    const std::uint32_t backgroundQuad = self->m_pBackgroundQuad;
    CSF_AT(void**, self, 0) = reinterpret_cast<void**>(CSF_VTBL(0x00865B40));

    if (backgroundQuad != 0)
    {
        ReleaseObjectReference(
            reinterpret_cast<void*>(backgroundQuad + 4u), nullptr);
        self->m_pBackgroundQuad = 0;
    }

    const std::uint32_t pointerQuad = self->m_pPointerQuad;
    if (pointerQuad != 0)
    {
        ReleaseObjectReference(
            reinterpret_cast<void*>(pointerQuad + 4u), nullptr);
        self->m_pPointerQuad = 0;
    }

    self->field_0148 = 1;

    const std::uint32_t childrenBegin = self->m_childrenBegin;
    std::uint32_t childCount = 0;
    if (childrenBegin != 0)
    {
        const std::uint32_t byteDifference =
            self->m_childrenEnd - childrenBegin;
        childCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        const std::uint32_t byteOffset = index * 4u;
        void* child = CSF_AT(
            void*, reinterpret_cast<void*>(self->m_childrenBegin), byteOffset);

        if (child != nullptr)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[0])(child, nullptr, 1);

            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->m_childrenBegin),
                   byteOffset) = 0;
        }
    }

    if (CSF_AT(std::uint32_t, self, 0x174) >= 0x10u)
    {
        void* stringStorage = CSF_AT(void*, self, 0x160);
        if (stringStorage != nullptr)
            Mem_FreeDebug(stringStorage);
    }

    CSF_AT(std::uint32_t, self, 0x174) = 15;
    CSF_AT(std::uint32_t, self, 0x170) = 0;
    CSF_AT(std::uint8_t, self, 0x160) = 0;

    const std::uint32_t childrenStorage = self->m_childrenBegin;
    if (childrenStorage != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(childrenStorage));

    self->m_childrenBegin = 0;
    self->m_childrenEnd = 0;
    self->field_0140 = 0;

    return CGuiControl_Destroy(
        reinterpret_cast<C_00830438*>(self), nullptr);
}

HOOK(0x0072BB50, CGuiPanel_Destroy, "auto");
