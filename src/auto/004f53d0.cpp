#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Commits an object-provider transform: builds a 0x20-byte local record
// (zeroed position/flags, three 1.0f scale components) and hands its address
// to the commit routine at 0x4F5310.
CSF_FUNC(char, __fastcall, sub_4F5310, 0x004F5310, (void* self, void* edx, int a3));

static char __fastcall ObjectProvider_Commit(void* self, void* /*edx*/)
{
    alignas(8) uint8_t commitBytes[0x20];

    CSF_AT(uint8_t, commitBytes, 0x1C) = 0;
    CSF_AT(uint8_t, commitBytes, 0x0D) = 0;
    CSF_AT(uint8_t, commitBytes, 0x0E) = 0;
    CSF_AT(uint32_t, commitBytes, 0x00) = 0;
    CSF_AT(uint32_t, commitBytes, 0x08) = 0;
    CSF_AT(float, commitBytes, 0x10) = 1.0f;
    CSF_AT(float, commitBytes, 0x14) = 1.0f;
    CSF_AT(float, commitBytes, 0x18) = 1.0f;
    CSF_AT(uint8_t, commitBytes, 0x0C) = 1;
    CSF_AT(uint32_t, commitBytes, 0x04) = 0;
    CSF_AT(uint8_t, commitBytes, 0x1D) = 1;

    return sub_4F5310(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(commitBytes)));
}

HOOK(0x004F53D0, ObjectProvider_Commit, "auto");
