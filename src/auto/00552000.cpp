#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, MpSpawnPoints_GetObjectId, 0x004A1980, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_IsTeamMode, 0x00550B30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ReportEvent, 0x00551680, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(char, __stdcall, MpSpawnPoints_NotifyTeam, 0x00551700, (char a1));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetGameMode, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, MpSpawnPoints_FindParticipant, 0x0073E420, (int (__stdcall *identifier)(int)));
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetVictimOutcome, 0x00740D20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetAttackerOutcome, 0x007411C0, (void* self, void* edx, int a2));

static char* __fastcall C_00844694_ProcessCombatEvent(
    C_00844694* self, void* /*edx*/, int event)
{
    struct {
        uint32_t id;
        C_00844694* owner;
    } scratch{0, self};

    const uint32_t selection = CSF_AT(
        uint32_t, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x4);
    void* modeOwner = selection
        ? reinterpret_cast<void*>(selection - 776)
        : nullptr;
    const int participantId = CSF_AT(int, reinterpret_cast<void*>(event), 0x2C);

    char* result = reinterpret_cast<char*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, event + 8));
    if (!result)
        return result;

    const int objectType = CSF_AT(int, result, 0x50);
    if (objectType == 0) {
        const uint32_t victimMember = CSF_AT(uint32_t, result, 0x18);
        void* victim = victimMember
            ? reinterpret_cast<void*>(victimMember - 1712)
            : nullptr;

        GetGlobalDword_73C020();
        result = reinterpret_cast<char*>(MpSpawnPoints_FindParticipant(
            reinterpret_cast<int (__stdcall *)(int)>(participantId)));

        void* attacker = reinterpret_cast<void*>(
            reinterpret_cast<uintptr_t>(result) - 1712);

        if (!result || reinterpret_cast<uintptr_t>(result) == 1712) {
            MpSpawnPoints_SetAttackerOutcome(victim, nullptr, 2);
            MpSpawnPoints_SetVictimOutcome(victim, nullptr, 2);
            result = MpSpawnPoints_GetGameMode(modeOwner, nullptr);
            if (result[6] == 3)
                return result;
            const uint32_t id = *MpSpawnPoints_GetObjectId(
                victim, nullptr, reinterpret_cast<uint32_t*>(&event));
            return reinterpret_cast<char*>(MpSpawnPoints_ReportEvent(
                scratch.owner, nullptr, 1112354867, static_cast<int>(id), -1, -1));
        }

        if (!victim)
            return result;

        if (attacker == victim) {
            MpSpawnPoints_SetAttackerOutcome(victim, nullptr, 2);
            MpSpawnPoints_SetVictimOutcome(victim, nullptr, 2);
            result = MpSpawnPoints_GetGameMode(modeOwner, nullptr);
            if (result[6] == 3)
                return result;
            const uint32_t id = *MpSpawnPoints_GetObjectId(
                victim, nullptr, reinterpret_cast<uint32_t*>(&event));
            return reinterpret_cast<char*>(MpSpawnPoints_ReportEvent(
                scratch.owner, nullptr, 1112354867, static_cast<int>(id), -1, -1));
        }

        char* mode = MpSpawnPoints_GetGameMode(modeOwner, nullptr);
        if (static_cast<uint8_t>(MpSpawnPoints_IsTeamMode(mode, nullptr))) {
            const uint8_t victimTeam = CSF_AT(uint8_t, victim, 0x443);
            const uint8_t attackerTeam = CSF_AT(uint8_t, attacker, 0x443);
            if (attackerTeam == victimTeam) {
                MpSpawnPoints_SetAttackerOutcome(attacker, nullptr, 0);
                MpSpawnPoints_SetVictimOutcome(victim, nullptr, 0);
                result = MpSpawnPoints_GetGameMode(modeOwner, nullptr);
                if (result[6] == 3)
                    return result;

                const uint32_t victimId = *MpSpawnPoints_GetObjectId(
                    victim, nullptr, reinterpret_cast<uint32_t*>(&event));
                uint32_t* attackerId = MpSpawnPoints_GetObjectId(
                    attacker, nullptr, &scratch.id);
                return reinterpret_cast<char*>(MpSpawnPoints_ReportEvent(
                    scratch.owner, nullptr, 1112354868,
                    static_cast<int>(*attackerId), static_cast<int>(victimId), -1));
            }

            MpSpawnPoints_SetAttackerOutcome(attacker, nullptr, 1);
            MpSpawnPoints_SetVictimOutcome(victim, nullptr, 1);
            if (MpSpawnPoints_GetGameMode(modeOwner, nullptr)[6] != 2 &&
                MpSpawnPoints_GetGameMode(modeOwner, nullptr)[6] != 3) {
                MpSpawnPoints_NotifyTeam(
                    static_cast<char>(CSF_AT(int8_t, attacker, 0x443)));
            }
        } else {
            MpSpawnPoints_SetAttackerOutcome(attacker, nullptr, 1);
            MpSpawnPoints_SetVictimOutcome(victim, nullptr, 1);
        }

        result = MpSpawnPoints_GetGameMode(modeOwner, nullptr);
        if (result[6] == 3)
            return result;

        const uint32_t victimId = *MpSpawnPoints_GetObjectId(
            victim, nullptr, reinterpret_cast<uint32_t*>(&event));
        uint32_t* attackerId = MpSpawnPoints_GetObjectId(
            attacker, nullptr, &scratch.id);
        return reinterpret_cast<char*>(MpSpawnPoints_ReportEvent(
            scratch.owner, nullptr, 1112354869,
            static_cast<int>(*attackerId), static_cast<int>(victimId), -1));
    }

    if (objectType == 31) {
        const uint32_t targetMember = CSF_AT(uint32_t, result, 0x18);
        void* target = targetMember
            ? reinterpret_cast<void*>(targetMember - 184)
            : nullptr;

        GetGlobalDword_73C020();
        result = reinterpret_cast<char*>(MpSpawnPoints_FindParticipant(
            reinterpret_cast<int (__stdcall *)(int)>(participantId)));
        if (!result || reinterpret_cast<uintptr_t>(result) == 1712 || !target)
            return result;

        void* attacker = reinterpret_cast<void*>(
            reinterpret_cast<uintptr_t>(result) - 1712);
        if (CSF_AT(uint8_t, attacker, 0x443) == CSF_AT(uint8_t, target, 0x5C)) {
            MpSpawnPoints_SetAttackerOutcome(attacker, nullptr, 0);
            const uint32_t id = *MpSpawnPoints_GetObjectId(
                attacker, nullptr, reinterpret_cast<uint32_t*>(&event));
            return reinterpret_cast<char*>(MpSpawnPoints_ReportEvent(
                scratch.owner, nullptr, 1112354871, static_cast<int>(id), -1, -1));
        }

        MpSpawnPoints_SetAttackerOutcome(attacker, nullptr, 1);
        const uint32_t id = *MpSpawnPoints_GetObjectId(
            attacker, nullptr, reinterpret_cast<uint32_t*>(&event));
        return reinterpret_cast<char*>(MpSpawnPoints_ReportEvent(
            scratch.owner, nullptr, 1112354870, static_cast<int>(id), -1, -1));
    }

    return result;
}

HOOK(0x00552000, C_00844694_ProcessCombatEvent, "auto");
