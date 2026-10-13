#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, GetSpawnFlagFromGlobal, 0x0073CD30, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_CharacterFacingDistance, 0x00833854);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int SubState_ResetOwner(float* self)
{
    void* owner = CSF_AT(void*, self, 0x00);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, owner, 0x00)[0x134 / 4])(
            owner, nullptr, 0, 0);
}

static int __fastcall SubState_UpdateTimedAction(
    float* self, void* /*edx*/, float elapsedTime)
{
    if (CSF_AT(int32_t, self, 0x2C) < 0)
        return SubState_ResetOwner(self);

    void* owner = CSF_AT(void*, self, 0x00);
    if (CSF_AT(uint8_t, owner, 0xBC) == 0)
        return SubState_ResetOwner(self);

    void* character = CSF_AT(void*, owner, 0x08);
    if (CSF_AT(uint32_t, character, 0x244) !=
        CSF_AT(uint32_t, self, 0x28))
        return SubState_ResetOwner(self);

    if (CSF_AT(float, character, 0x258) > g_CharacterFacingDistance)
        return SubState_ResetOwner(self);

    int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(
            reinterpret_cast<uint8_t*>(character) + 0x240)));

    if (resolved != 0)
    {
        uint32_t objectAddress =
            CSF_AT(uint32_t, reinterpret_cast<void*>(resolved), 0x18);
        if (objectAddress != 0)
        {
            objectAddress -= 128;
            if (objectAddress != 0)
            {
                int spawnFlag = CSF_AT(
                    int32_t, reinterpret_cast<void*>(objectAddress), 0x5C);
                if (spawnFlag != -1)
                {
                    int globalObject = GetGlobalDword_73C020();
                    if (spawnFlag != GetSpawnFlagFromGlobal(
                            reinterpret_cast<void*>(globalObject), nullptr))
                        return SubState_ResetOwner(self);
                }
            }
        }
    }

    float remainingTime = CSF_AT(float, self, 0x1C) - elapsedTime;
    CSF_AT(float, self, 0x1C) = remainingTime;

    if (remainingTime < g_fZero)
    {
        uint32_t targetHandle = CSF_AT(uint32_t, self, 0x28);
        int32_t actionIndex = CSF_AT(int32_t, self, 0x2C);

        alignas(8) uint8_t recordBytes[0x34]{};
        CSF_AT(void**, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(void**, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(int32_t, recordBytes, 0x18) = -1;
        CSF_AT(int32_t, recordBytes, 0x1C) = -1;
        CSF_AT(void**, recordBytes, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(uint32_t, recordBytes, 0x04) = 33;
        std::memset(recordBytes + 0x20, 0, 16);
        CSF_AT(uint8_t, recordBytes, 0x30) = 0;
        CSF_AT(uint32_t, recordBytes, 0x0C) = targetHandle;
        CSF_AT(int32_t, recordBytes, 0x10) = actionIndex;

        int dispatcher = GetGlobalObjectField184();
        DispatchRecordToGlobalObject(
            reinterpret_cast<void*>(dispatcher), nullptr, recordBytes, 0);
        return SubState_ResetOwner(self);
    }

    float progress =
        g_MaxPercent - remainingTime / CSF_AT(float, self, 0x20) * g_MaxPercent;
    int state = GetGlobalState_50D190();
    CSF_AT(float, reinterpret_cast<void*>(state), 0xFC) = progress;
    return state;
}

HOOK(0x0048DDA0, SubState_UpdateTimedAction, "auto");
