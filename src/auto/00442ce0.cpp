#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __fastcall, CHud_GetLetterboxBounds, 0x0042DFC0, (CHud* self, void* , uint32_t* outTop, uint32_t* outBottom));
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall DynamicObj_UpdateScreenRectangle(void* self)
{
    alignas(8) uint8_t screenSizeBytes[0xC];
    CSF_AT(float, screenSizeBytes, 0) = 0.0f;
    CSF_AT(float, screenSizeBytes, 4) = 0.0f;

    GetGlobal8afc78();
    Hud_WriteNormalizedScreenSize(reinterpret_cast<float*>(screenSizeBytes));

    void* screen = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    int topOffset = 0;
    int height = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x24 / 4])(screen, nullptr);

    if (static_cast<uint8_t>(MissionInfo_IsPrerequisiteMet()) != 0)
    {
        void* letterboxState = CSF_AT(void*, g_pCurrentScene, 0xC0);
        if (letterboxState && CSF_AT(uint8_t, letterboxState, 9) != 0)
        {
            alignas(8) uint8_t bottomBytes[0x8];
            alignas(8) uint8_t topBytes[0x4];

            CHud* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
            CHud_GetLetterboxBounds(
                hud, nullptr,
                reinterpret_cast<uint32_t*>(topBytes),
                reinterpret_cast<uint32_t*>(bottomBytes));

            void* top = reinterpret_cast<void*>(
                CSF_AT(uint32_t, topBytes, 0));
            void* bounds = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, top, 0)[0x28 / 4])(top, nullptr);

            topOffset = static_cast<int32_t>(static_cast<int64_t>(
                static_cast<float>(CSF_AT(int32_t, bounds, 4) - 1) *
                CSF_AT(float, screenSizeBytes, 4)));

            top = reinterpret_cast<void*>(CSF_AT(uint32_t, topBytes, 0));
            bounds = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, top, 0)[0x28 / 4])(top, nullptr);

            float topHeight =
                static_cast<float>(CSF_AT(int32_t, bounds, 4) - 1) *
                CSF_AT(float, screenSizeBytes, 4);

            void* bottom = reinterpret_cast<void*>(
                CSF_AT(uint32_t, bottomBytes, 0));
            bounds = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, bottom, 0)[0x28 / 4])(bottom, nullptr);

            float bottomHeight =
                static_cast<float>(CSF_AT(int32_t, bounds, 4) - 2) *
                CSF_AT(float, screenSizeBytes, 4);

            height = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, screen, 0)[0x24 / 4])(screen, nullptr);
            height -= static_cast<int32_t>(
                static_cast<int64_t>(bottomHeight + topHeight));
        }
    }

    int width = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x20 / 4])(screen, nullptr);

    void* destination = CSF_AT(void*, self, 0x24);

    alignas(8) uint8_t rectangleBytes[0x10];
    CSF_AT(int32_t, rectangleBytes, 0) = 0;
    CSF_AT(int32_t, rectangleBytes, 4) = topOffset;
    CSF_AT(int32_t, rectangleBytes, 8) = width;
    CSF_AT(int32_t, rectangleBytes, 0xC) = height + 3;

    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int32_t*)>(
        CSF_AT(void**, destination, 0)[0x1C / 4])(
            destination, nullptr, 4,
            reinterpret_cast<int32_t*>(rectangleBytes));
}

HOOK(0x00442CE0, DynamicObj_UpdateScreenRectangle, "auto");
