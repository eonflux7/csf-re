#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies an action record from a2 (source) into this object (self), then copies the embedded
// sub-state at +0x38 via WeaponFirearms_CopyState. Returns self.
CSF_FUNC(std::uint32_t*, __fastcall, WeaponFirearms_CopyState, 0x004759B0, (std::uint32_t* self, void* , const void* source));

static std::uint32_t* __fastcall ActionRecord_CopyFrom(std::uint32_t* self, void* /*edx*/, const void* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);
    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) = CSF_AT(std::uint32_t, source, 0x0C);
    CSF_AT(std::uint32_t, self, 0x14) = CSF_AT(std::uint32_t, source, 0x14);
    CSF_AT(std::uint32_t, self, 0x18) = CSF_AT(std::uint32_t, source, 0x18);
    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint8_t, self, 0x28) = CSF_AT(std::uint8_t, source, 0x28);
    CSF_AT(std::uint8_t, self, 0x29) = CSF_AT(std::uint8_t, source, 0x29);
    CSF_AT(std::uint8_t, self, 0x2A) = CSF_AT(std::uint8_t, source, 0x2A);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);

    WeaponFirearms_CopyState(
        reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0x38),
        nullptr,
        reinterpret_cast<const void*>(reinterpret_cast<const std::uint8_t*>(source) + 0x38));

    return self;
}

HOOK(0x0048A090, ActionRecord_CopyFrom, "auto");
