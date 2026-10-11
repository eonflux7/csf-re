#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall CharacterModel_AdjustClampedRotation(
    void* self, void* /*edx*/, float horizontalDelta, float verticalDelta)
{
    if (horizontalDelta == g_fZero && verticalDelta == g_fZero)
        return;

    void* controller = CSF_AT(void*, self, 0x04);
    void* rotation =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x10 / 4])(controller, nullptr);

    float horizontal = static_cast<float>(
        ((double (__fastcall*)(void*, void*))
            CSF_AT(void**, rotation, 0)[0x24 / 4])(rotation, nullptr)
        + horizontalDelta);

    float vertical = static_cast<float>(
        ((double (__fastcall*)(void*, void*))
            CSF_AT(void**, rotation, 0)[0x28 / 4])(rotation, nullptr)
        + verticalDelta);

    float upperHorizontal =
        CSF_AT(float, self, 0x84) + CSF_AT(float, self, 0xA4);
    if (horizontal > upperHorizontal)
        horizontal = upperHorizontal;

    float lowerHorizontal =
        CSF_AT(float, self, 0x84) - CSF_AT(float, self, 0xA4);
    if (horizontal < lowerHorizontal)
        horizontal = lowerHorizontal;

    float upperVertical =
        CSF_AT(float, self, 0x88) + CSF_AT(float, self, 0xA8);
    if (vertical > upperVertical)
        vertical = upperVertical;

    float lowerVertical =
        CSF_AT(float, self, 0x88) - CSF_AT(float, self, 0xA8);
    if (vertical < lowerVertical)
        vertical = lowerVertical;

    ((void (__fastcall*)(void*, void*, float))
        CSF_AT(void**, rotation, 0)[0x30 / 4])(rotation, nullptr, horizontal);
    ((void (__fastcall*)(void*, void*, float))
        CSF_AT(void**, rotation, 0)[0x34 / 4])(rotation, nullptr, vertical);
}

HOOK(0x004F4F50, CharacterModel_AdjustClampedRotation, "auto");
