#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, SceneObject_GetDrawState, 0x0049CF10,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneObject_NotifyDepleted, 0x0049D400,
         (void* self, void* edx, void* a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, C_0083b770_GetRelatedObject, 0x004D0AA0,
         (void* self, void* edx));

static void __fastcall C_0083b770_UpdateCountdown(
    C_0083b770* self, void* /*edx*/, float elapsedTime)
{
    self->field_0040 -= elapsedTime;
    if (self->field_0040 > g_fZero)
        return;

    uint32_t* relatedObject = C_0083b770_GetRelatedObject(self, nullptr);
    if (relatedObject)
    {
        void* drawState = reinterpret_cast<void*>(
            SceneObject_GetDrawState(
                reinterpret_cast<void*>(self->field_0014), nullptr));

        alignas(8) uint8_t eventBytes[0x2C];
        CSF_AT(int32_t, eventBytes, 0x04) = -1;
        CSF_AT(int32_t, eventBytes, 0x08) = -1;
        CSF_AT(int32_t, eventBytes, 0x0C) = -1;

        uint8_t* position =
            reinterpret_cast<uint8_t*>(self->field_0014) + 0x1C;

        CSF_AT(uint32_t, eventBytes, 0x10) = 0;
        CSF_AT(int32_t, eventBytes, 0x14) = 0;
        CSF_AT(uint32_t, eventBytes, 0x28) = 0;
        CSF_AT(void**, eventBytes, 0x00) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, eventBytes, 0x18) = 0;
        CSF_AT(uint32_t, eventBytes, 0x1C) = 0;
        CSF_AT(uint32_t, eventBytes, 0x20) = 0;
        CSF_AT(uint32_t, eventBytes, 0x24) = 0;

        CSF_AT(uint32_t, eventBytes, 0x04) =
            CSF_AT(uint32_t, position, 0x04);
        CSF_AT(uint32_t, eventBytes, 0x08) =
            CSF_AT(uint32_t, position, 0x08);
        CSF_AT(uint32_t, eventBytes, 0x0C) =
            CSF_AT(uint32_t, drawState, 0x70);
        CSF_AT(uint32_t, eventBytes, 0x10) =
            CSF_AT(uint32_t, drawState, 0x114);

        uint32_t finalState = CSF_AT(uint32_t, drawState, 0x114);
        void* receiver = CSF_AT(void*, relatedObject, 0x38);
        CSF_AT(uint32_t, eventBytes, 0x28) = finalState;
        CSF_AT(int32_t, eventBytes, 0x14) =
            CSF_AT(uint32_t, relatedObject, 0x50) != 4 ? -1 : 20;

        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, receiver, 0)[0x8C / 4])(
                receiver, nullptr, eventBytes);
    }

    alignas(8) uint8_t directionBytes[0xC];
    void* sceneObject = reinterpret_cast<void*>(self->field_0014);
    CSF_AT(float, directionBytes, 0x00) = 0.0f;
    CSF_AT(float, directionBytes, 0x04) = 1.0f;
    CSF_AT(float, directionBytes, 0x08) = 0.0f;
    SceneObject_NotifyDepleted(sceneObject, nullptr, directionBytes, 0);

    sceneObject = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, sceneObject, 0)[0xA0 / 4])(
            sceneObject, nullptr);

    self->field_0044 = 3;
}

HOOK(0x004D0C50, C_0083b770_UpdateCountdown, "auto");
