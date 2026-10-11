#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __stdcall, NetGathering_TestPeerObject, 0x00799C20, (void** a1));

// Returns non-zero when the object's name matches "Commandos3DGameGathering",
// or when the peer-object test accepts it.
static int __stdcall NetGathering_IsGatheringNameOrHandled(int a1)
{
    // Temporary string object lives in the 4-byte stack slot [ebp-4].
    alignas(4) std::uint8_t tempStringBytes[4];

    InitializeTemporaryString(
        tempStringBytes,
        nullptr,
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(CSF_STR(0x0086000C, "Commandos3DGameGathering"))));

    char nameMatches = NameMatchesString(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(a1)),
        nullptr,
        reinterpret_cast<void**>(tempStringBytes));

    DestroyTemporaryString(tempStringBytes, nullptr);

    if (nameMatches)
        return 1;

    char peerAccepted = NetGathering_TestPeerObject(
        reinterpret_cast<void**>(static_cast<std::uintptr_t>(a1)));

    if (peerAccepted)
        return 1;

    return 0;
}

HOOK(0x6AB3C0, NetGathering_IsGatheringNameOrHandled, "auto");
