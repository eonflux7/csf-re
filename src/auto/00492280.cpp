#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, S_004a0490_UpdateAlertEntry, 0x004A0940, (S_004a0490* self, void* , int referenceId, int iconState, int p4, int p5, int p6));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_00833b88_UpdateAlertIndicator(
    C_00833b88* self, void* /*edx*/, int /*unused*/)
{
    if (self->field_0050 != 1)
        return;

    void* source = reinterpret_cast<void*>(self->field_0034);
    if (!source)
        return;

    if (((int (__fastcall*)(void*, void*))
         CSF_AT(void**, source, 0)[0x98 / 4])(source, nullptr) != 1)
        return;

    source = reinterpret_cast<void*>(self->field_0034);
    const uint8_t specialAlert = CSF_AT(uint8_t, source, 0x33C);
    const bool active = ((bool (__fastcall*)(void*, void*))
                        CSF_AT(void**, source, 0)[0xB0 / 4])(source, nullptr);
    if (!specialAlert && !active)
        return;

    alignas(8) uint8_t primaryColorBytes[0x4] = {};
    alignas(8) uint8_t secondaryColorBytes[0xC] = {};
    uint8_t* color = secondaryColorBytes + 4;
    int iconState = -1;
    float progress = 0.0f;

    if (specialAlert)
    {
        CSF_AT(int, primaryColorBytes, 0) = -2147418113;
        CSF_AT(int, secondaryColorBytes, 0) = -2147418368;
        CSF_AT(int, secondaryColorBytes, 4) = -2147418368;
        iconState = 0;
        if (CSF_AT(float, source, 0x338) > g_fZero)
            progress = CSF_AT(float, source, 0x334) /
                       CSF_AT(float, source, 0x338);
    }
    else
    {
        uint8_t* timer =
            reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, source, 0x30C)) + 0x690;
        const int state = CSF_AT(int, self, 0x1D8);
        if (state == 1)
        {
            iconState = 0;
            void* alphaSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* redSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* greenSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* blueSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            const uint8_t blue = CSF_AT(uint8_t, blueSource, 8782);
            const uint8_t green = CSF_AT(uint8_t, greenSource, 8781);
            color[0] = blue;
            const uint8_t red = CSF_AT(uint8_t, redSource, 8780);
            color[1] = green;
            const uint8_t alpha = CSF_AT(uint8_t, alphaSource, 8783);
            color[2] = red;
            color[3] = alpha;
            std::memcpy(primaryColorBytes, color, 4);

            alphaSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            redSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            greenSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            blueSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            const uint8_t secondaryBlue = CSF_AT(uint8_t, blueSource, 8778);
            const uint8_t secondaryGreen = CSF_AT(uint8_t, greenSource, 8777);
            color[0] = secondaryBlue;
            const uint8_t secondaryRed = CSF_AT(uint8_t, redSource, 8776);
            color[1] = secondaryGreen;
            const uint8_t secondaryAlpha = CSF_AT(uint8_t, alphaSource, 8779);
            color[2] = secondaryRed;
            color[3] = secondaryAlpha;
            std::memcpy(secondaryColorBytes, color, 4);
        }
        else if (state == 2)
        {
            iconState = 1;
            void* alphaSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* redSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* greenSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* blueSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            const uint8_t blue = CSF_AT(uint8_t, blueSource, 8790);
            const uint8_t green = CSF_AT(uint8_t, greenSource, 8789);
            color[0] = blue;
            const uint8_t red = CSF_AT(uint8_t, redSource, 8788);
            color[1] = green;
            const uint8_t alpha = CSF_AT(uint8_t, alphaSource, 8791);
            color[2] = red;
            color[3] = alpha;
            std::memcpy(primaryColorBytes, color, 4);

            alphaSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            redSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            greenSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            blueSource = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            const uint8_t secondaryBlue = CSF_AT(uint8_t, blueSource, 8786);
            const uint8_t secondaryGreen = CSF_AT(uint8_t, greenSource, 8785);
            color[0] = secondaryBlue;
            const uint8_t secondaryRed = CSF_AT(uint8_t, redSource, 8784);
            color[1] = secondaryGreen;
            const uint8_t secondaryAlpha = CSF_AT(uint8_t, alphaSource, 8787);
            color[2] = secondaryRed;
            color[3] = secondaryAlpha;
            std::memcpy(secondaryColorBytes, color, 4);
        }

        if (state == 1 || state == 2)
        {
            if (CSF_AT(float, timer, 8) > g_fZero)
                progress = CSF_AT(float, timer, 4) / CSF_AT(float, timer, 8);
        }
    }

    std::memcpy(secondaryColorBytes + 8, &progress, sizeof(progress));
    S_004a0490_UpdateAlertEntry(
        CSF_AT(S_004a0490*, g_pCurrentScene, 0xAC),
        nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x1C),
        iconState,
        CSF_AT(int, secondaryColorBytes, 8),
        reinterpret_cast<int>(primaryColorBytes),
        reinterpret_cast<int>(secondaryColorBytes));
}

HOOK(0x00492280, C_00833b88_UpdateAlertIndicator, "auto");
