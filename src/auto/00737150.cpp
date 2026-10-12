#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall C_00867260_DrawWithHalfAlpha(
    C_00867260* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x20))
    {
        const float originalAlpha = CSF_AT(float, self, 0x74);
        int originalBits;
        std::memcpy(&originalBits, &originalAlpha, sizeof(originalBits));

        void** vtable = CSF_AT(void**, self, 0);
        CSF_AT(float, self, 0x74) = originalAlpha * g_HalfDurationScale;
        reinterpret_cast<void (__fastcall*)(void*, void*)>(vtable[2])(
            self, nullptr);
        CSF_AT(int, self, 0x74) = originalBits;
        return originalBits;
    }

    for (int offset = 0; offset < 112; offset += 28)
    {
        const uint8_t alpha = static_cast<uint8_t>(
            static_cast<int64_t>(
                CSF_AT(float, self, 0x74) * g_HalfDurationScale));
        const uint8_t red = static_cast<uint8_t>(
            static_cast<int64_t>(CSF_AT(float, self, 0x68)));
        const uint8_t green = static_cast<uint8_t>(
            static_cast<int64_t>(CSF_AT(float, self, 0x6C)));
        const uint8_t blue = static_cast<uint8_t>(
            static_cast<int64_t>(CSF_AT(float, self, 0x70)));

        uint8_t* vertices = CSF_AT(uint8_t*, self, 0x14);
        CSF_AT(uint32_t, vertices + offset, 0x10) =
            (static_cast<uint32_t>(alpha) << 24) |
            (static_cast<uint32_t>(red) << 16) |
            (static_cast<uint32_t>(green) << 8) |
            static_cast<uint32_t>(blue);
    }

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[2])(self, nullptr);

    uint8_t* vertices = nullptr;
    for (int offset = 0; offset < 112; offset += 28)
    {
        const uint8_t alpha = static_cast<uint8_t>(
            static_cast<int64_t>(CSF_AT(float, self, 0x74)));
        const uint8_t red = static_cast<uint8_t>(
            static_cast<int64_t>(CSF_AT(float, self, 0x68)));
        const uint8_t green = static_cast<uint8_t>(
            static_cast<int64_t>(CSF_AT(float, self, 0x6C)));
        const uint8_t blue = static_cast<uint8_t>(
            static_cast<int64_t>(CSF_AT(float, self, 0x70)));

        vertices = CSF_AT(uint8_t*, self, 0x14);
        CSF_AT(uint32_t, vertices + offset, 0x10) =
            (static_cast<uint32_t>(alpha) << 24) |
            (static_cast<uint32_t>(red) << 16) |
            (static_cast<uint32_t>(green) << 8) |
            static_cast<uint32_t>(blue);
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(vertices));
}

HOOK(0x00737150, C_00867260_DrawWithHalfAlpha, "auto");
