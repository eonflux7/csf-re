#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_00555e40_SendSyncEvent, 0x005567F0,
         (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_UpdatePrimaryState, 0x0074A210,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_UpdateSecondaryState, 0x0074A230,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetSyncEntry, 0x0074A520,
         (void* self, void* edx, unsigned int a2));

static char __fastcall S_00555e40_SynchronizeEntries(
    S_00555e40* self, void* /*edx*/, char sendEvents)
{
    void* globals = reinterpret_cast<void*>(GetGlobalDword_73C020());
    const bool enabled = self->field_14a0 != 0;
    void* manager = CSF_AT(void*, globals, 0xC8);
    char result = static_cast<char>(reinterpret_cast<std::uintptr_t>(manager));

    if (enabled && manager)
    {
        for (int index = 0; index < 30; ++index)
        {
            char* entry = MpSpawnPoints_GetSyncEntry(manager, nullptr, index);
            if (!entry)
            {
                result = 0;
                continue;
            }

            const int identifier = CSF_AT(int, entry, 0x0);
            if (identifier == -1)
            {
                result = static_cast<char>(identifier);
                continue;
            }

            if (!CSF_AT(std::uint8_t, entry, 0x2A))
            {
                if (sendEvents)
                    S_00555e40_SendSyncEvent(
                        self, nullptr, 1398353968, identifier, -1, -1);

                MpSpawnPoints_UpdatePrimaryState(manager, nullptr, index);
            }

            result = static_cast<char>(CSF_AT(std::uint8_t, entry, 0x29));
            if (!result)
            {
                result = static_cast<char>(CSF_AT(std::uint8_t, entry, 0x2B));
                if (!result)
                {
                    if (sendEvents)
                        S_00555e40_SendSyncEvent(
                            self, nullptr, 1398353969,
                            CSF_AT(int, entry, 0x0), -1, -1);

                    result = static_cast<char>(
                        MpSpawnPoints_UpdateSecondaryState(manager, nullptr, index));
                }
            }
        }
    }

    return result;
}

HOOK(0x00556CF0, S_00555e40_SynchronizeEntries, "auto");
