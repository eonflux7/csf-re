#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCore_AdvanceCharacterTimer, 0x004AEC90, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, AiCore_DebugPrintFinIa, 0x004B90C0, (int a1, char* Format, ...));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiCore_TickAiFrame(void* self, void* /*edx*/, int a2, float a3)
{
    uint32_t remaining = CSF_AT(uint32_t, self, 0x6E0);
    void* timerOwner = static_cast<uint8_t*>(self) - 4;

    if (remaining != 0)
    {
        uint32_t nextRemaining = remaining - 1;
        float accumulated = a3 + CSF_AT(float, timerOwner, 0x6E0);
        CSF_AT(uint32_t, timerOwner, 0x6E4) = nextRemaining;
        CSF_AT(uint8_t, timerOwner, 0x6E8) = 1;
        CSF_AT(float, timerOwner, 0x6E0) = accumulated;

        if (nextRemaining == 0)
        {
            AiCore_DebugPrintFinIa(
                reinterpret_cast<int>(timerOwner),
                const_cast<char*>(CSF_STR(0x0083A9E0, "-- Fin Debug IA --")));
        }
    }

    if (CSF_AT(int, self, 0x6D4) >= 0)
    {
        void* handle = static_cast<uint8_t*>(self) + 0x6CC;
        if (CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                 reinterpret_cast<int>(handle)) == 0)
        {
            CSF_AT(int, handle, 0x4) = -1;
            CSF_AT(int, handle, 0x8) = -1;
        }
    }

    int result = CharActionFsm_IsWeaponActionBlocked(
        CSF_AT(void*, self, 0x28), nullptr);
    if (static_cast<uint8_t>(result) == 0)
    {
        int timeBits;
        std::memcpy(&timeBits, &a3, sizeof(timeBits));
        AiCoverAttackTasks_UpdateCharacterTasks(a2, timeBits);
        AiCore_AdvanceCharacterTimer(timerOwner, nullptr, a3);
        return 0;
    }
    return result;
}

HOOK(0x004BE6B0, AiCore_TickAiFrame, "auto");
