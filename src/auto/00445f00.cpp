#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall GetSceneCandidateList(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0xE8) = 1;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    return reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0xD8);
}

HOOK(0x445F00, GetSceneCandidateList, "auto");
