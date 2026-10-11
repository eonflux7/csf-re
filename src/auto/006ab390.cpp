#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Binds to the original temporary-string helpers. The original uses the value
// returned in eax (the temporary string), so it is typed as returning void*.
CSF_FUNC(void*, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));

// Returns nonzero when the object at a1 is named "Commandos3DGameGathering".
static char __stdcall NetGathering_MatchesGatheringName(int a1)
{
    // Stack object at [ebp-4], 4 bytes of room in the original frame.
    alignas(8) std::uint8_t nameBytes[4];
    C_008310b8& var4 = *reinterpret_cast<C_008310b8*>(nameBytes);

    void* tempString = InitializeTemporaryString(
        &var4,
        nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(CSF_STR(0x86000C, "Commandos3DGameGathering"))));

    char matches = NameMatchesString(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(a1)),
        nullptr,
        reinterpret_cast<void**>(tempString));

    DestroyTemporaryString(&var4, nullptr);
    return matches;
}

HOOK(0x006AB390, NetGathering_MatchesGatheringName, "auto");
