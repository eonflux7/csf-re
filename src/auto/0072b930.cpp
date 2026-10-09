#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetPosition_7198A0, 0x007198A0, (CGuiControl* self, void* edx, float x, float y));

static int __fastcall CGuiPanel_SetPosition(CGuiPanel* self, void* /*edx*/, float x, float y)
{
    const uint32_t childrenBegin = self->m_childrenBegin;
    const uint32_t childCount = childrenBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->m_childrenEnd - childrenBegin) >> 2)
        : 0;

    for (uint32_t i = 0; i < childCount; ++i)
    {
        auto* child = reinterpret_cast<CGuiControl**>(self->m_childrenBegin)[i];
        child->vSetPosition_54(x, y);
    }

    void* pointerQuad = reinterpret_cast<void*>(self->m_pPointerQuad);
    if (pointerQuad)
    {
        auto* dimensions =
            reinterpret_cast<int32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, pointerQuad, 0)[0x3C / 4])(pointerQuad, nullptr);

        int32_t scaledDimensions[2] = {dimensions[0], dimensions[1]};
        scaledDimensions[0] = static_cast<int32_t>(
            static_cast<int64_t>(static_cast<double>(scaledDimensions[0]) * x));
        scaledDimensions[1] = static_cast<int32_t>(
            static_cast<int64_t>(static_cast<double>(scaledDimensions[1]) * y));

        pointerQuad = reinterpret_cast<void*>(self->m_pPointerQuad);
        reinterpret_cast<void (__fastcall*)(void*, void*, int32_t*)>(
            CSF_AT(void**, pointerQuad, 0)[0x38 / 4])(
                pointerQuad, nullptr, scaledDimensions);
    }

    return CGuiControl_SetPosition_7198A0(self, nullptr, x, y);
}

HOOK(0x0072B930, CGuiPanel_SetPosition, "auto");
