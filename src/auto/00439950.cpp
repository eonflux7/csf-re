#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(char*, g_pszKey_TEX_V0_899680, 0x00899680);
CSF_GLOBAL(char*, g_pszKey_TEX_U0_899684, 0x00899684);
CSF_GLOBAL(char*, g_pszKey_TEX_V1_899688, 0x00899688);
CSF_GLOBAL(char*, g_pszKey_TEX_U1_89968C, 0x0089968C);

// Fills a 4-float UV rectangle {u0, v0, u1, v1} from the texture properties
// of `props`. A null `props` yields an all-zero rectangle.
static float* __cdecl TexUv_ReadRectFromProperties(float* uvRect, S_00692b80* props)
{
    if (props == nullptr)
    {
        uvRect[0] = 0.0f;
        uvRect[2] = 0.0f;
        uvRect[3] = 0.0f;
        uvRect[1] = 0.0f;
        return uvRect;
    }

    auto* propsObj = reinterpret_cast<std::uint8_t*>(props);

    float v0 = static_cast<float>(ReadFloatProperty(propsObj, nullptr, g_pszKey_TEX_V0_899680));
    float u0 = static_cast<float>(ReadFloatProperty(propsObj, nullptr, g_pszKey_TEX_U0_899684));
    float v1 = static_cast<float>(ReadFloatProperty(propsObj, nullptr, g_pszKey_TEX_V1_899688));
    float u1 = static_cast<float>(ReadFloatProperty(propsObj, nullptr, g_pszKey_TEX_U1_89968C));

    uvRect[0] = u0;
    uvRect[2] = u1;
    uvRect[3] = v1;
    uvRect[1] = v0;
    return uvRect;
}

HOOK(0x00439950, TexUv_ReadRectFromProperties, "auto");
