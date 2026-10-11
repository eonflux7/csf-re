#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0082f9f8_StartMovement(
    C_0082f9f8* self, void* /*edx*/, int movement)
{
    const std::uint32_t movementAddress = static_cast<std::uint32_t>(movement);
    const std::uint32_t previousAddress = CSF_AT(std::uint32_t, self, 0x10);

    if (movementAddress != previousAddress)
    {
        if (previousAddress)
        {
            void* previous = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(previousAddress));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, previous, 0)[0x94 / 4])(previous, nullptr);
        }

        CSF_AT(std::uint32_t, self, 0x10) = movementAddress;
        if (movement)
        {
            void* next = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(movementAddress));
            CSF_AT(std::uint32_t, next, 0x08) =
                CSF_AT(std::uint32_t, self, 0x08);

            void* current = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x10)));
            CSF_AT(C_0082f9f8*, current, 0x0C) = self;

            current = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x10)));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, current, 0)[0x8C / 4])(current, nullptr);
        }
    }

    return movement;
}

HOOK(0x004DB140, C_0082f9f8_StartMovement, "auto");
