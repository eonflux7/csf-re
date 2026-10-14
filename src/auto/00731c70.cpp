#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00867078_RenderQuad, 0x00731850,
         (void* self, void* edx, int* a2, float a3));
CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0,
         (void* self, void* edx, int effect));

static bool __fastcall C_00867078_ProcessAnimatedQuad(
    C_00867078* self, void* /*edx*/, int* quad)
{
    if (!quad)
        return false;

    // 731B75..731B86 (and 731BE5..731BF6) copy four dwords to
    // the output. The original passes ebp-0x44, overlapping ebp-0x3C.
    // Neither getter takes the saved EBX as an argument: both end in ret 4.
    // Keep the complete frame backing these objects, without inventing values
    // for geometry outputs that the virtual getters are responsible for filling.
    alignas(8) uint8_t temporaryBytes[0x44];
    auto* effectValue = reinterpret_cast<uint32_t*>(temporaryBytes);
    auto* originalScale = reinterpret_cast<float*>(temporaryBytes + 0x8);
    auto* originalPosition = reinterpret_cast<int32_t*>(temporaryBytes + 0x18);
    auto* originalSize = reinterpret_cast<int32_t*>(temporaryBytes + 0x20);
    auto* scaledScale = reinterpret_cast<float*>(temporaryBytes + 0x28);
    auto* scaledSize = reinterpret_cast<int32_t*>(temporaryBytes + 0x30);
    auto* position = reinterpret_cast<int32_t*>(temporaryBytes + 0x38);

    int effect = CanConfigureEffect(self, nullptr, 4);
    if (effect && CSF_AT(uint8_t, reinterpret_cast<void*>(effect), 0x4))
    {
        position[2] = self->vSlot14();
        const float scale = static_cast<float>(self->vSlot1C());

        ((void(__fastcall*)(void*, void*, int32_t*))
             CSF_AT(void**, quad, 0)[0x54 / 4])(
            quad, nullptr, originalPosition);
        ((void(__fastcall*)(void*, void*, int32_t*))
             CSF_AT(void**, quad, 0)[0x5C / 4])(
            quad, nullptr, originalSize);

        scaledSize[1] = originalSize[1];
        scaledSize[0] = static_cast<int32_t>(static_cast<int64_t>(
            static_cast<double>(originalSize[0]) * scale));

        const int32_t widthDifference = static_cast<int32_t>(
            static_cast<uint32_t>(scaledSize[0]) -
            static_cast<uint32_t>(originalSize[0]));
        const int32_t centerX = static_cast<int32_t>(
            static_cast<uint32_t>(originalPosition[0]) +
            static_cast<uint32_t>(widthDifference / -2));
        CSF_AT(int32_t, originalScale, 0xC) = originalPosition[1];

        ((void(__fastcall*)(void*, void*, int32_t*))
             CSF_AT(void**, quad, 0)[0x58 / 4])(
            quad, nullptr, scaledSize);

        float* currentScale =
            ((float*(__fastcall*)(void*, void*))
                 CSF_AT(void**, quad, 0)[0x9C / 4])(quad, nullptr);
        originalScale[0] = currentScale[0];
        originalScale[1] = currentScale[1];
        scaledScale[0] = originalScale[0] * scale;
        scaledScale[1] = originalScale[1];

        ((void(__fastcall*)(void*, void*, float*))
             CSF_AT(void**, quad, 0)[0x98 / 4])(
            quad, nullptr, scaledScale);

        position[0] = static_cast<int32_t>(
            static_cast<uint32_t>(centerX) -
            static_cast<uint32_t>(position[2]));
        position[1] = CSF_AT(int32_t, originalScale, 0xC);
        ((void(__fastcall*)(void*, void*, int32_t*))
             CSF_AT(void**, quad, 0)[0x50 / 4])(
            quad, nullptr, position);
        C_00867078_RenderQuad(self, nullptr, quad, 1.0f);

        position[0] = static_cast<int32_t>(
            static_cast<uint32_t>(centerX) +
            static_cast<uint32_t>(position[2]));
        position[1] = CSF_AT(int32_t, originalScale, 0xC);
        ((void(__fastcall*)(void*, void*, int32_t*))
             CSF_AT(void**, quad, 0)[0x50 / 4])(
            quad, nullptr, position);
        C_00867078_RenderQuad(self, nullptr, quad, 1.0f);

        ((void(__fastcall*)(void*, void*, int32_t*))
             CSF_AT(void**, quad, 0)[0x50 / 4])(
            quad, nullptr, originalPosition);
        ((void(__fastcall*)(void*, void*, int32_t*))
             CSF_AT(void**, quad, 0)[0x58 / 4])(
            quad, nullptr, originalSize);
        ((void(__fastcall*)(void*, void*, float*))
             CSF_AT(void**, quad, 0)[0x98 / 4])(
            quad, nullptr, originalScale);
        return true;
    }

    effect = CanConfigureEffect(self, nullptr, 2);
    if (effect && CSF_AT(uint8_t, reinterpret_cast<void*>(effect), 0x4))
    {
        void** quadVtable = CSF_AT(void**, quad, 0);
        uint32_t* value = self->vSlotC(effectValue);
        ((void(__fastcall*)(void*, void*, uint32_t*))quadVtable[0x60 / 4])(
            quad, nullptr, value);
        C_00867078_RenderQuad(self, nullptr, quad, 1.0f);
        return true;
    }

    effect = CanConfigureEffect(self, nullptr, 3);
    if (effect && CSF_AT(uint8_t, reinterpret_cast<void*>(effect), 0x4))
    {
        void** quadVtable = CSF_AT(void**, quad, 0);
        uint32_t* value = self->vSlot10(effectValue);
        ((void(__fastcall*)(void*, void*, uint32_t*))quadVtable[0x60 / 4])(
            quad, nullptr, value);
        C_00867078_RenderQuad(self, nullptr, quad, 1.0f);
        return true;
    }

    effect = CanConfigureEffect(self, nullptr, 0);
    if (!effect || !CSF_AT(uint8_t, reinterpret_cast<void*>(effect), 0x4))
        return false;

    const float opacity = static_cast<float>(self->vSlot4());
    C_00867078_RenderQuad(self, nullptr, quad, opacity);
    return true;
}

HOOK(0x00731C70, C_00867078_ProcessAnimatedQuad, "auto");
