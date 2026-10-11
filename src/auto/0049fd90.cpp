#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));

static int32_t __fastcall HudAlertIcons_UpdateIcon(
    void* self, void* /*edx*/, void* iconRecord, void* source,
    int32_t image, int32_t option)
{
    if (!image)
    {
        void* icon = CSF_AT(void*, iconRecord, 0xC);
        if (!icon)
            return 0;

        int32_t result =
            ((int32_t(__fastcall*)(void*, void*, int32_t))
                CSF_AT(void**, icon, 0)[0])(icon, nullptr, 1);
        CSF_AT(void*, iconRecord, 0xC) = nullptr;
        return result;
    }

    if (!CSF_AT(void*, iconRecord, 0xC))
    {
        void* manager = CSF_AT(void*, self, 0xC);
        CSF_AT(void*, iconRecord, 0xC) =
            ((void*(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, manager, 0)[0x54 / 4])(
                    manager, nullptr, source);
    }

    void* icon = CSF_AT(void*, iconRecord, 0xC);
    ((void(__fastcall*)(void*, void*, int32_t))
        CSF_AT(void**, icon, 0)[0x40 / 4])(icon, nullptr, image);

    if (option)
    {
        icon = CSF_AT(void*, iconRecord, 0xC);
        ((void(__fastcall*)(void*, void*, int32_t))
            CSF_AT(void**, icon, 0)[0x60 / 4])(icon, nullptr, option);
    }

    alignas(8) uint8_t screenSizeBytes[0x8];
    float* screenSize = reinterpret_cast<float*>(screenSizeBytes);
    screenSize[0] = 0.0f;
    screenSize[1] = 0.0f;
    GetGlobal8afc78();
    Hud_WriteNormalizedScreenSize(screenSize);

    icon = CSF_AT(void*, iconRecord, 0xC);
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, icon, 0)[0x98 / 4])(icon, nullptr, screenSize);

    alignas(8) uint8_t boundsBytes[0x10];
    uint32_t* bounds = reinterpret_cast<uint32_t*>(boundsBytes);
    bounds[0] = 0;
    bounds[1] = 0;
    bounds[2] = 0;
    bounds[3] = 0;

    int32_t height =
        ((int32_t(__fastcall*)(void*, void*, int32_t))
            CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr, 1);
    bounds[3] = bounds[1] + static_cast<uint32_t>(height);

    alignas(8) uint8_t sizeBytes[0x8];
    icon = CSF_AT(void*, iconRecord, 0xC);
    uint32_t* size =
        ((uint32_t*(__fastcall*)(void*, void*, void*, int32_t))
            CSF_AT(void**, icon, 0)[0x78 / 4])(
                icon, nullptr, sizeBytes, 0);
    bounds[2] = size[0] + bounds[0];

    icon = CSF_AT(void*, iconRecord, 0xC);
    return ((int32_t(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, icon, 0)[0x48 / 4])(icon, nullptr, bounds);
}

HOOK(0x0049FD90, HudAlertIcons_UpdateIcon, "auto");
