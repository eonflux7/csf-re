#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_SpawnTimerThreshold, 0x008446C8);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SpawnPoints_InitializeRequest, 0x00550AA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_FinalizeRequest, 0x00554690, (void* self, void* edx));
CSF_FUNC(bool, __stdcall, SpawnPoints_IsSpawnBlocked, 0x00554F40, (int a1));
CSF_FUNC(void*, __fastcall, SpawnPoints_SubmitRequest, 0x00555E40, (void* self, void* edx, int a2));

static int __fastcall S_00555e40_UpdateSpawnTimer(
    S_00555e40* self, void* /*edx*/, float elapsedTime)
{
    // The packet also labels this direct-call address as a vtable.
    (void)CSF_VTBL(0x00555E40);

    uint32_t selection = 0;
    if (g_CharacterSelectState8B0694) {
        const uint32_t selectionLink = CSF_AT(
            uint32_t, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x4);
        if (selectionLink)
            selection = selectionLink - 776;
    }

    const uint32_t ownerLink = CSF_AT(
        uint32_t, reinterpret_cast<void*>(self->field_0c8c), 0x18);
    int result = ownerLink ? static_cast<int>(ownerLink - 1712) : 0;

    alignas(4) uint8_t request[0x44]{};
    CSF_AT(int, request, 0x40) = result;

    if (!selection || !result)
        return result;

    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x443))
        return result;

    const int firstHandle = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x1404));
    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x1410));

    if (!firstHandle || !result)
        return result;

    const uint32_t firstLink =
        CSF_AT(uint32_t, reinterpret_cast<void*>(firstHandle), 0x18);
    const uint32_t firstObject = firstLink ? firstLink - 128 : 0;
    const uint32_t secondLink =
        CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x18);
    result = secondLink ? static_cast<int>(secondLink - 128) : 0;

    if (!firstObject || !result)
        return result;

    CSF_AT(int, request, 0x20) = 7;
    CSF_AT(int, request, 0x1C) = 0;
    CSF_AT(int16_t, request, 0xC) = 0;
    SpawnPoints_InitializeRequest(request, nullptr);

    const bool timerExpired = self->field_1434 > g_SpawnTimerThreshold;
    CSF_AT(uint8_t, request, 0x28) = 1;
    CSF_AT(int, request, 0x24) = 1;

    if (!timerExpired) {
        self->field_1434 = elapsedTime + self->field_1434;
    } else {
        const int owner = CSF_AT(int, request, 0x40);
        self->field_1434 = 0.0f;
        if (!SpawnPoints_IsSpawnBlocked(
                CSF_AT(int8_t, reinterpret_cast<void*>(owner), 0x443))) {
            CSF_AT(int, request, 0x2C) = 1094987830;
            SpawnPoints_SubmitRequest(
                self, nullptr, reinterpret_cast<int>(request));
            return SpawnPoints_FinalizeRequest(request, nullptr);
        }
    }

    return SpawnPoints_FinalizeRequest(request, nullptr);
}

HOOK(0x005561D0, S_00555e40_UpdateSpawnTimer, "auto");
