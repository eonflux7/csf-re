#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MpSpawnPoints_GetPendingSpawn, 0x007415C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetStateTime, 0x00748700, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(void, __fastcall, MpSpawnPoints_CreateSpawn, 0x0048C370,
         (void* self, void* edx, uint32_t* position, uint32_t* direction,
          int count, float spread, uint32_t type, void* parameters, int direct));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static int __stdcall MpSpawnPoints_ProcessPendingSpawns(void* owner)
{
    alignas(4) uint8_t parameters[0x34] = {};
    uint32_t normalizedTransform[6];
    uint32_t position[3];
    uint32_t direction[3];

    for (;;)
    {
        int result = MpSpawnPoints_GetPendingSpawn(owner, nullptr);
        if (!result)
            return result;

        void* spawn = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        uint32_t state = g_CharacterSelectState8B0694;
        const uint32_t lastTimeLow = CSF_AT(uint32_t, owner, 0x838);
        const uint32_t lastTimeHigh = CSF_AT(uint32_t, owner, 0x83C);

        result = -1;
        if (state)
        {
            uint32_t linked = CSF_AT(uint32_t, reinterpret_cast<void*>(state), 0x4);
            if (linked)
            {
                linked -= 776;
                if (linked)
                {
                    result = MpSpawnPoints_GetStateTime(reinterpret_cast<void*>(linked), nullptr);
                    state = g_CharacterSelectState8B0694;
                }
            }
        }

        const uint64_t lastTime =
            (static_cast<uint64_t>(lastTimeHigh) << 32) | lastTimeLow;
        if ((lastTimeHigh || lastTimeLow >= 0x32) &&
            static_cast<uint64_t>(static_cast<int64_t>(result)) >= lastTime - 50)
            return result;

        int currentTime = -1;
        if (state)
        {
            uint32_t linked = CSF_AT(uint32_t, reinterpret_cast<void*>(state), 0x4);
            if (linked)
            {
                linked -= 776;
                if (linked)
                    currentTime = MpSpawnPoints_GetStateTime(reinterpret_cast<void*>(linked), nullptr);
            }
        }

        CSF_AT(int, owner, 0x838) = currentTime;
        CSF_AT(int, owner, 0x83C) = currentTime < 0 ? -1 : 0;

        position[1] = CSF_AT(uint32_t, spawn, 0x4);
        position[0] = CSF_AT(uint32_t, spawn, 0x0);
        position[2] = CSF_AT(uint32_t, spawn, 0x8);
        direction[1] = CSF_AT(uint32_t, spawn, 0x10);
        direction[0] = CSF_AT(uint32_t, spawn, 0xC);
        direction[2] = CSF_AT(uint32_t, spawn, 0x14);

        normalizedTransform[0] = direction[0];
        normalizedTransform[1] = direction[1];
        normalizedTransform[2] = direction[2];
        normalizedTransform[3] = position[0];
        normalizedTransform[4] = position[1];
        normalizedTransform[5] = position[2];
        NormalizeVector3(normalizedTransform, nullptr);

        const int handleValue =
            GetValueIfNotFlagged(static_cast<uint8_t*>(owner) + 0x6B0, nullptr);
        void* source = handleValue
            ? reinterpret_cast<void*>(static_cast<uintptr_t>(handleValue) - 20)
            : nullptr;

        CSF_AT(void**, parameters, 0x0) = CSF_VTBL(0x0082D448);
        CSF_AT(void**, parameters, 0x10) = CSF_VTBL(0x0082D448);
        CSF_AT(int, parameters, 0x14) = -1;
        CSF_AT(int, parameters, 0x18) = -1;
        CSF_AT(int, parameters, 0x1C) = 0;
        CSF_AT(int, parameters, 0x20) = 0;
        CSF_AT(int, parameters, 0x24) = 0;
        CSF_AT(int, parameters, 0xC) = 0;
        CSF_AT(int, parameters, 0x28) = -1;
        CSF_AT(uint8_t, parameters, 0x2C) = 1;
        CSF_AT(uint8_t, parameters, 0x2D) = 0;
        CSF_AT(int, parameters, 0x4) = -1;
        CSF_AT(int, parameters, 0x8) = -1;
        CSF_AT(uint8_t, parameters, 0x2E) = 0;
        CSF_AT(uint8_t, parameters, 0x2F) = 0;
        CSF_AT(uint8_t, parameters, 0x30) = 1;
        CSF_AT(uint8_t, parameters, 0x31) = 0;

        CSF_AT(uint32_t, parameters, 0x4) = CSF_AT(uint32_t, source, 0x20);
        CSF_AT(uint32_t, parameters, 0x8) = CSF_AT(uint32_t, source, 0x24);
        CSF_AT(uint32_t, parameters, 0x28) = CSF_AT(uint32_t, source, 0x78);

        const uint8_t hasCount = CSF_AT(uint8_t, spawn, 0x1C);
        CSF_AT(uint8_t, parameters, 0x2D) = 0;
        CSF_AT(uint8_t, parameters, 0x2C) = 1;
        CSF_AT(int, parameters, 0xC) = 2;
        CSF_AT(int, parameters, 0x14) = -1;
        CSF_AT(int, parameters, 0x18) = -1;
        CSF_AT(int, parameters, 0x1C) = 0;
        CSF_AT(int, parameters, 0x20) = 0;
        CSF_AT(int, parameters, 0x24) = 0;

        if (hasCount)
        {
            const uint32_t type = CSF_AT(uint32_t, spawn, 0x18);
            const float spread = CSF_AT(float, spawn, 0x20);
            const int count = CSF_AT(int8_t, spawn, 0x1D);
            void* manager = CSF_AT(void*, g_pCurrentScene, 0xA8);
            MpSpawnPoints_CreateSpawn(manager, nullptr, position, direction,
                                      count, spread, type, parameters, 0);
        }
        else
        {
            const uint32_t type = CSF_AT(uint32_t, spawn, 0x18);
            const float spread = CSF_AT(float, spawn, 0x20);
            void* manager = CSF_AT(void*, g_pCurrentScene, 0xA8);
            MpSpawnPoints_CreateSpawn(manager, nullptr, position, direction,
                                      1, spread, type, parameters, 1);
        }

        CSF_AT(void**, parameters, 0x10) = CSF_VTBL(0x0082D410);
        CSF_AT(void**, parameters, 0x0) = CSF_VTBL(0x0082D410);
    }
}

HOOK(0x00552FB0, MpSpawnPoints_ProcessPendingSpawns, "auto");
