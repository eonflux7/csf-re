#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_MessageBoxAnimationDuration, 0x0082DC84);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);
CSF_GLOBAL(float, g_MessageBoxAlphaScale, 0x00841270);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall CMenuMessageBox_UpdateOpeningAnimation(
    CMenuMessageBox* self, void* /*edx*/, float deltaTime)
{
    // An unordered comparison also enters the animation branch.
    if (CSF_AT(float, self, 0x974) >= g_MessageBoxAnimationDuration)
    {
        self->field_0028 = 1;
        return;
    }

    void* panel = reinterpret_cast<void*>(self->field_0894);
    void* boundsObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, panel, 0)[0x1CC / 4])(panel, nullptr);
    if (!boundsObject)
        return;

    const uint32_t targetX = self->field_0988;
    const uint32_t initialY = self->field_097c;
    const uint32_t initialX = self->field_0978;
    const int32_t differenceX = static_cast<int32_t>(initialX - targetX);
    const int32_t differenceY =
        static_cast<int32_t>(initialY - self->field_098c);

    // The original spills the horizontal displacement to float, but keeps
    // the vertical displacement in the floating-point register stack.
    const float displacementX = static_cast<float>(
        static_cast<double>(differenceX) * CSF_AT(float, self, 0x974) *
        g_MessageBoxAnimationScale);
    const double displacementY =
        static_cast<double>(differenceY) * CSF_AT(float, self, 0x974) *
        g_MessageBoxAnimationScale;

    uint32_t position[2];
    position[0] = initialX -
        static_cast<uint32_t>(static_cast<int64_t>(displacementX));
    position[1] = initialY -
        static_cast<uint32_t>(static_cast<int64_t>(displacementY));

    const uint32_t widthOffset = static_cast<uint32_t>(
        static_cast<int64_t>(
            static_cast<double>(displacementX) * g_MessageBoxSizeScale));
    const double heightDisplacement =
        displacementY * g_MessageBoxSizeScale;
    uint32_t size[2];
    size[0] = self->field_0980 - widthOffset;
    size[1] = self->field_0984 -
        static_cast<uint32_t>(static_cast<int64_t>(heightDisplacement));

    uint32_t limit = self->field_0988;
    if (static_cast<int32_t>(position[0]) <= static_cast<int32_t>(limit))
        position[0] = limit;

    limit = self->field_098c;
    if (static_cast<int32_t>(position[1]) <= static_cast<int32_t>(limit))
        position[1] = limit;

    limit = self->field_0990;
    if (static_cast<int32_t>(size[0]) >= static_cast<int32_t>(limit))
        size[0] = limit;

    limit = self->field_0994;
    if (static_cast<int32_t>(size[1]) >= static_cast<int32_t>(limit))
        size[1] = limit;

    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, boundsObject, 0)[0x1C / 4])(
            boundsObject, nullptr, position);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, boundsObject, 0)[0x24 / 4])(
            boundsObject, nullptr, size);

    uint32_t rectangle[4];
    rectangle[0] = position[0];
    rectangle[1] = position[1];
    rectangle[2] = position[0] + size[0];
    rectangle[3] = position[1] + size[1];

    panel = reinterpret_cast<void*>(self->field_0894);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*, int)>(
        CSF_AT(void**, panel, 0)[0x208 / 4])(
            panel, nullptr, rectangle, 0);

    panel = reinterpret_cast<void*>(self->field_0894);
    void* colorObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, panel, 0)[0x1DC / 4])(panel, nullptr);
    if (colorObject)
    {
        const float alpha = self->field_0949
            ? static_cast<float>(
                static_cast<double>(CSF_AT(float, self, 0x974)) *
                g_MessageBoxAlphaScale)
            : g_fZero;
        float color[4] = {0.0f, 0.0f, 0.0f, alpha};
        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, colorObject, 0)[0x2C / 4])(
                colorObject, nullptr, color);
    }

    CSF_AT(float, self, 0x974) =
        deltaTime + CSF_AT(float, self, 0x974);
}

HOOK(0x00528C20, CMenuMessageBox_UpdateOpeningAnimation, "auto");
