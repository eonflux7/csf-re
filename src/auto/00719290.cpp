#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_FUNC(std::int64_t, __cdecl, TruncateX87ToInt64, 0x007AF86C, ());

// __ftol2 takes its operand in ST(0), not on the C++ argument stack.
// Reproduce its integer result, including the low word of its invalid result.
static std::uint32_t TruncateAnimationDimension(float value)
{
    (void)TruncateX87ToInt64;

    const double widened = static_cast<double>(value);
    if (!(widened >= -9223372036854775808.0 &&
          widened <   9223372036854775808.0))
    {
        return 0;
    }

    return static_cast<std::uint32_t>(
        static_cast<std::int64_t>(widened));
}

static void __fastcall CGuiControl_SetSizeAnimation(
    CGuiControl* self, void* /*edx*/, void* animation, char preserveCenter)
{
    void* previousAnimation = reinterpret_cast<void*>(self->m_pSizeAnim);
    if (previousAnimation)
    {
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, previousAnimation, 0)[0])(
                previousAnimation, nullptr, 1);
        self->m_pSizeAnim = 0;
    }

    self->m_sizeAnimTime = 0.0f;
    self->m_pSizeAnim = reinterpret_cast<std::uint32_t>(animation);

    if (!animation)
        return;

    if (preserveCenter)
    {
        void* positionAnimation = reinterpret_cast<void*>(self->m_pPosAnim);
        if (positionAnimation)
        {
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, positionAnimation, 0)[0])(
                    positionAnimation, nullptr, 1);
            self->m_pPosAnim = 0;
        }

        const std::int32_t* position =
            ((const std::int32_t* (__fastcall*)(void*, void*))
                self->vtbl[0x9C / 4])(self, nullptr);

        const std::int32_t* size =
            ((const std::int32_t* (__fastcall*)(void*, void*))
                self->vtbl[0xA4 / 4])(self, nullptr);

        void* generatedPositionAnimation =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, animation, 0)[0x04 / 4])(
                    animation, nullptr);

        ((void (__fastcall*)(void*, void*))
            CSF_AT(void**, generatedPositionAnimation, 0)[0x1C / 4])(
                generatedPositionAnimation, nullptr);

        const std::uint32_t keyCount =
            ((std::uint32_t (__fastcall*)(void*, void*))
                CSF_AT(void**, animation, 0)[0x20 / 4])(
                    animation, nullptr);

        const std::int32_t halfWidth = size[0] / 2;
        const std::uint32_t centerXBits =
            static_cast<std::uint32_t>(halfWidth) +
            static_cast<std::uint32_t>(position[0]);

        const std::int32_t halfHeight = size[1] / 2;
        const std::uint32_t centerYBits =
            static_cast<std::uint32_t>(halfHeight) +
            static_cast<std::uint32_t>(position[1]);

        if (keyCount != 0)
        {
            const float centerX =
                static_cast<float>(static_cast<std::int32_t>(centerXBits));
            const float centerY =
                static_cast<float>(static_cast<std::int32_t>(centerYBits));

            std::uint32_t keyIndex = 0;
            do
            {
                const float* keySize =
                    ((const float* (__fastcall*)(void*, void*, std::uint32_t))
                        CSF_AT(void**, animation, 0)[0x24 / 4])(
                            animation, nullptr, keyIndex);

                const float keyWidth = keySize[0];
                const float keyHeight = keySize[1];

                const float keyTime = static_cast<float>(
                    ((double (__fastcall*)(void*, void*, std::uint32_t))
                        CSF_AT(void**, animation, 0)[0x28 / 4])(
                            animation, nullptr, keyIndex));

                float keyPosition[2];
                keyPosition[0] = static_cast<float>(
                    static_cast<double>(centerX) -
                    static_cast<double>(keyWidth) * g_HalfDurationScale);
                keyPosition[1] = static_cast<float>(
                    static_cast<double>(centerY) -
                    static_cast<double>(keyHeight) * g_HalfDurationScale);

                ((void (__fastcall*)(void*, void*, float*, float))
                    CSF_AT(void**, generatedPositionAnimation, 0)[0x18 / 4])(
                        generatedPositionAnimation, nullptr,
                        keyPosition, keyTime);

                ++keyIndex;
            }
            while (keyIndex < keyCount);
        }

        ((void (__fastcall*)(void*, void*, void*))
            self->vtbl[0x128 / 4])(
                self, nullptr, generatedPositionAnimation);
    }

    float initialSize[2];
    ((void (__fastcall*)(void*, void*, float*, std::uint32_t))
        CSF_AT(void**, animation, 0)[0x14 / 4])(
            animation, nullptr, initialSize, 0);

    std::uint32_t integerSize[2];
    integerSize[0] = TruncateAnimationDimension(initialSize[0]);
    integerSize[1] = TruncateAnimationDimension(initialSize[1]);

    ((void (__fastcall*)(void*, void*, std::uint32_t*))
        self->vtbl[0xA0 / 4])(self, nullptr, integerSize);
}

HOOK(0x00719290, CGuiControl_SetSizeAnimation, "auto");
