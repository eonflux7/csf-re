#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall SceneNodeQuery_Initialize(
    std::uint32_t* self, void* /*edx*/, int queryType,
    std::uint32_t* coordinates, int argument4, int argument5, int argument6)
{
    CSF_AT(std::uint32_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x00) = queryType;
    CSF_AT(std::uint32_t, self, 0x04) = coordinates[0];
    CSF_AT(std::uint32_t, self, 0x08) = coordinates[1];
    CSF_AT(std::uint32_t, self, 0x0C) = coordinates[2];
    CSF_AT(std::uint32_t, self, 0x14) = argument5;
    CSF_AT(std::uint32_t, self, 0x10) = argument4;
    CSF_AT(std::uint32_t, self, 0x18) = argument6;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    return self;
}

HOOK(0x004458A0, SceneNodeQuery_Initialize, "auto");
