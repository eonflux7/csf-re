#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static unsigned int __fastcall C_00861ce0_SetTextureFrame(
    C_00861ce0* self, void* /*edx*/, unsigned int frame)
{
    unsigned int result = 0;
    float horizontalStep = 0.0f;
    float verticalStep = 0.0f;

    void* resource = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_000c), 0x18);

    if (resource)
    {
        void* holder = CSF_AT(void*, resource, 0x20);
        void* first = CSF_AT(void*, holder, 0x00);
        if (first)
        {
            void* second = CSF_AT(void*, first, 0x00);
            if (second)
            {
                void* geometry = CSF_AT(void*, second, 0x00);
                unsigned int width = CSF_AT(unsigned int, geometry, 0x0C);
                unsigned int height = CSF_AT(unsigned int, geometry, 0x10);

                if (width == height)
                {
                    result = 1;
                }
                else if (width <= height)
                {
                    result = height / width;
                    verticalStep = static_cast<float>(
                        g_TransitionOne / static_cast<double>(result));
                }
                else
                {
                    result = width / height;
                    horizontalStep = static_cast<float>(
                        g_TransitionOne / static_cast<double>(result));
                }
            }
        }
    }

    if (self->field_0028 != frame && frame < result)
    {
        float verticalOffset = static_cast<float>(
            verticalStep * static_cast<double>(frame));
        float horizontalOffset = static_cast<float>(
            static_cast<double>(frame) * horizontalStep);

        result = static_cast<unsigned int>(
            reinterpret_cast<std::uintptr_t>(
                self->vSlot54(horizontalOffset, verticalOffset)));
        self->field_0028 = static_cast<std::uint8_t>(frame);
    }

    return result;
}

HOOK(0x006E5870, C_00861ce0_SetTextureFrame, "auto");
