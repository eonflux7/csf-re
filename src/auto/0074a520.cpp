#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_FindParticipant, 0x0073E420, (int (__stdcall *participantHandle)(int)));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Returns the per-slot entry (this + 48*slot + 4) and refreshes its participant data.
// Slot must be in [0, 0x1E); anything else returns nullptr.
static char* __fastcall MpSpawnPoints_GetPlayerEntry(char* self, void* /*edx*/, int32_t a2)
{
    if (a2 < 0 || a2 >= 0x1E)
    {
        return nullptr;
    }

    char* entry = self + 48 * a2 + 4;
    if (entry == nullptr)
    {
        return entry;
    }

    const int32_t participantHandle = CSF_AT(int32_t, entry, 0);
    GetGlobalDword_73C020();
    const int32_t participant = NetSession_FindParticipant(
        reinterpret_cast<int (__stdcall *)(int)>(static_cast<std::uintptr_t>(participantHandle)));

    if (participant != 0 && participant != 0x6B0)
    {
        char* account = reinterpret_cast<char*>(
            static_cast<std::uintptr_t>(static_cast<std::uint32_t>(participant) - 0x6B0u));

        CSF_AT(int32_t, entry, 0x24) = CSF_AT(int32_t, account, 0x674);

        const std::uint8_t nameFlag = CSF_AT(std::uint8_t, account, 0x44C);
        if (nameFlag != CSF_AT(std::uint8_t, entry, 0x2C))
        {
            CSF_AT(std::uint8_t, entry, 0x2C) = nameFlag;
            // ecx at the call is the account pointer (last written at 0x74A556: lea ecx, [eax-0x6B0]).
            void* playerName = MenuMpAccounts_GetPlayerName(account, nullptr);
            AssignStringRange_488830(entry + 4, nullptr, playerName, 0, 0xFFFFFFFFu);
        }
    }

    return entry;
}

HOOK(0x0074A520, MpSpawnPoints_GetPlayerEntry, "auto");
