#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char*, __fastcall, NetSession_FormatTimestampField, 0x0073C800, (char* self, void*));
CSF_FUNC(int, __fastcall, S_00749b70_GetPlayerId, 0x00749B70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_00749d70_FindObjectIndex, 0x00749D70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_00749b70_FindPlayerCheckSlot, 0x00749F30, (void* self, void* edx, int a2));

[[maybe_unused]] static const auto kAnnotatedPlayerTable = CSF_VTBL(0x00749B70);
[[maybe_unused]] static const auto kAnnotatedObjectTable = CSF_VTBL(0x00749D70);

static bool __fastcall S_00749b70_IsPlayerCheckComplete(
    S_00749b70* self, void* /*edx*/)
{
    bool complete = false;

    for (int index = 0; index < 16; ++index)
        CSF_AT(std::uint8_t, self, 0x08 + 137 + index * 8) = 0;

    for (int index = 0; index < 16; ++index)
    {
        const int playerId = S_00749b70_GetPlayerId(self, nullptr, index);

        // The getter preserves ECX, so the search receives the same object.
        if (S_00749d70_FindObjectIndex(self, nullptr, playerId) == -1)
        {
            const int slot = S_00749b70_FindPlayerCheckSlot(self, nullptr, playerId);
            bool slotComplete = false;

            if (slot != -1)
            {
                const std::uint8_t attempts = static_cast<std::uint8_t>(
                    CSF_AT(std::uint8_t, self, 0x08 + 136 + slot * 8) + 1);

                CSF_AT(std::int32_t, self, 0x08 + 132 + slot * 8) = playerId;
                CSF_AT(std::uint8_t, self, 0x08 + 136 + slot * 8) = attempts;
                CSF_AT(std::uint8_t, self, 0x08 + 137 + slot * 8) = 1;

                NetSession_FormatTimestampField(
                    reinterpret_cast<char*>(GetGlobalDword_73C020()), nullptr);

                const std::int8_t currentAttempts =
                    CSF_AT(std::int8_t, self, 0x08 + 136 + slot * 8);
                slotComplete =
                    (currentAttempts > 11 &&
                     static_cast<std::uint32_t>(playerId) < self->field_010c) ||
                    currentAttempts > 40;
            }

            complete = slotComplete;
        }
    }

    for (int index = 0; index < 16; ++index)
    {
        if (!CSF_AT(std::uint8_t, self, 0x08 + 137 + index * 8))
        {
            CSF_AT(std::int32_t, self, 0x08 + 132 + index * 8) = -1;
            CSF_AT(std::uint8_t, self, 0x08 + 136 + index * 8) = 0;
            CSF_AT(std::uint8_t, self, 0x08 + 137 + index * 8) = 1;
        }
    }

    return complete;
}

HOOK(0x0074A0B0, S_00749b70_IsPlayerCheckComplete, "auto");
