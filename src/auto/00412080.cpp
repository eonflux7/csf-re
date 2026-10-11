#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, GetProviderDirectionVector, 0x00411E70, (int a1));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MovementScale, 0x0082E034);

static void __fastcall BeginPendingMovement(void* self, void* /*edx*/, float elapsedTime)
{
    if (CSF_AT(std::uint8_t, self, 0x88) &&
        !CSF_AT(std::uint8_t, self, 0x89) &&
        !CSF_AT(std::uint8_t, self, 0x8B) &&
        CSF_AT(float, self, 0xD4) == g_fZero)
    {
        CSF_AT(std::uint8_t, self, 0x88) = 0;
        CSF_AT(std::uint8_t, self, 0x8A) = 0;
        void* object = CSF_AT(void*, self, 0x04);
        CSF_AT(std::uint8_t, self, 0x8C) = 1;
        CSF_AT(float, self, 0x84) =
            elapsedTime * g_MovementScale * CSF_AT(float, self, 0x13C) +
            CSF_AT(float, self, 0x84);

        const void* position =
            reinterpret_cast<const void*>(GetObjectPositionVector(object, nullptr));
        CSF_AT(std::uint32_t, self, 0xDC) =
            CSF_AT(std::uint32_t, position, 0x04);

        std::uint32_t directionBuffer[3];
        const void* direction = reinterpret_cast<const void*>(
            GetProviderDirectionVector(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(directionBuffer))));
        CSF_AT(std::uint32_t, self, 0x24) =
            CSF_AT(std::uint32_t, direction, 0x00);
        CSF_AT(std::uint32_t, self, 0x28) =
            CSF_AT(std::uint32_t, direction, 0x04);
        CSF_AT(std::uint32_t, self, 0x2C) =
            CSF_AT(std::uint32_t, direction, 0x08);
    }
}

HOOK(0x00412080, BeginPendingMovement, "auto");
