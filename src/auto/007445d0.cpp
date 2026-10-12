#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(uint8_t*, __fastcall, C_00867900_Construct, 0x00741B20, (C_00867900* self, void*));

static uint8_t* __cdecl NetPlayerDDL_AllocateAndInit()
{
    void* storage = AllocateGatheringStorage(
        2112, CSF_STR(0x00867B48, ".\\PCGenerados\\NetPlayerDDL.cpp"), 11);
    if (!storage)
        return nullptr;

    return C_00867900_Construct(static_cast<C_00867900*>(storage), nullptr);
}

HOOK(0x007445D0, NetPlayerDDL_AllocateAndInit, "auto");
