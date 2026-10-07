#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

static float* __fastcall CGuiControl_GetEffectiveColor(
    CGuiControl* self, void* /*edx*/, float* output)
{
    auto* parent = reinterpret_cast<CGuiControl*>(self->m_pParent);
    if (parent)
    {
        float parentColorStorage[4];
        const float* parentColor =
            reinterpret_cast<float* (__fastcall*)(CGuiControl*, void*, float*)>(
                parent->vtbl[0x228 / 4])(parent, nullptr, parentColorStorage);

        const double alpha =
            static_cast<double>(parentColor[3]) * self->m_colorA;
        const double blue =
            static_cast<double>(parentColor[2]) * self->m_colorB;
        const double green =
            static_cast<double>(parentColor[1]) * self->m_colorG;
        const double red =
            static_cast<double>(parentColor[0]) * self->m_colorR;

        output[0] = static_cast<float>(red);
        output[1] = static_cast<float>(green);
        output[2] = static_cast<float>(blue);
        output[3] = static_cast<float>(alpha);
    }
    else
    {
        std::memcpy(output + 0, &self->m_colorR, sizeof(float));
        std::memcpy(output + 1, &self->m_colorG, sizeof(float));
        std::memcpy(output + 2, &self->m_colorB, sizeof(float));
        std::memcpy(output + 3, &self->m_colorA, sizeof(float));
    }

    return output;
}

HOOK(0x00719050, CGuiControl_GetEffectiveColor, "auto");
