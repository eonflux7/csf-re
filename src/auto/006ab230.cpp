#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));

// Initializes the temporary string of a gathering object with "Commandos3DGameGathering" and returns the object.
static C_008310b8* __stdcall InitGatheringTemporaryString(C_008310b8* a1)
{
    InitializeTemporaryString(a1, nullptr, reinterpret_cast<std::int32_t>(CSF_STR(0x0086000C, "Commandos3DGameGathering")));
    return a1;
}

HOOK(0x006AB230, InitGatheringTemporaryString, "auto");
