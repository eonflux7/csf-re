#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionY, 0x004D1B50, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(void, __cdecl, NetSpawnMessages_SetPlayerExcluded, 0x00743DE0, (int a1, char a2));
CSF_FUNC(void, __cdecl, _qsort, 0x007AEAE0, (void* base, std::size_t count, std::size_t size, int (__cdecl* compare)(const void*, const void*)));

CSF_GLOBAL(float, g_SelectionRefreshInterval, 0x00868AB8);
CSF_GLOBAL(float, g_SelectionOriginX, 0x008BC1D0);
CSF_GLOBAL(float, g_SelectionOriginY, 0x008BC1D4);
CSF_GLOBAL(float, g_SelectionOriginZ, 0x008BC1D8);

static void* __fastcall NetSpawnMessages_UpdatePlayerSelection(
    void* self, void* /*edx*/, void* context)
{
    void* result = CSF_AT(void*, context, 0x0);
    if (!result)
        return result;

    int value = GetValueIfNotFlagged(
        static_cast<std::uint8_t*>(result) + 0x6B0, nullptr);
    result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));
    if (!value)
        return result;

    result = CSF_AT(void*, context, 0x4);
    if (!result)
        return result;

    void* clock = CSF_AT(void*, self, 0x4);
    double now = reinterpret_cast<double (__fastcall*)(void*, void*)>(
        CSF_AT(void**, clock, 0x0)[0x10 / 4])(clock, nullptr);

    bool refresh;
    if (now - CSF_AT(float, self, 0x8) < g_SelectionRefreshInterval)
    {
        refresh = false;
    }
    else
    {
        CSF_AT(float, self, 0x8) = static_cast<float>(now);
        refresh = true;
    }

    alignas(8) std::uint8_t candidateBytes[0x94];
    auto* candidates = reinterpret_cast<void**>(candidateBytes);
    std::size_t candidateCount = 0;

    result = CSF_AT(void*, CSF_AT(void*, context, 0x4), 0x4);
    void* node = CSF_AT(void*, result, 0x0);
    if (node == result)
        return result;

    do
    {
        void* player = CSF_AT(void*, node, 0x8);
        if (player != CSF_AT(void*, context, 0x0) &&
            GetValueIfNotFlagged(
                static_cast<std::uint8_t*>(player) + 0x6B0, nullptr))
        {
            candidates[candidateCount++] = player;
        }
        node = CSF_AT(void*, node, 0x0);
    }
    while (node != CSF_AT(void*, CSF_AT(void*, context, 0x4), 0x4));

    result = reinterpret_cast<void*>(candidateCount);
    if (static_cast<std::int32_t>(candidateCount) <= 0)
        return result;

    float positionZ = static_cast<float>(
        GetPlayerPositionZ(CSF_AT(void*, context, 0x0), nullptr));
    float positionY = static_cast<float>(
        GetPlayerPositionY(CSF_AT(void*, context, 0x0), nullptr));
    g_SelectionOriginX = static_cast<float>(
        GetPlayerPositionX(CSF_AT(void*, context, 0x0), nullptr));

    std::int32_t sortedCount = static_cast<std::int32_t>(candidateCount);
    g_SelectionOriginY = positionY;
    g_SelectionOriginZ = positionZ;
    _qsort(candidates, candidateCount, 4u,
        reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
            CSF_ADDR_FN(0x00751B50)));

    result = CSF_AT(void*, context, 0x0);
    int sourceTeam = CSF_AT(std::int8_t, result, 0x443);
    std::int32_t acceptedCount = 0;

    if (sortedCount > 0)
    {
        std::int32_t index = 0;
        do
        {
            char checkTeam = CSF_AT(char, context, 0xC);
            void* player = candidates[index];
            char excluded = 0;

            if (checkTeam)
                excluded = CSF_AT(std::int8_t, player, 0x443) != sourceTeam;

            if (!excluded)
            {
                void* filter = CSF_AT(void*, context, 0x8);
                if (filter)
                {
                    excluded = reinterpret_cast<char (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, filter, 0x0)[0x1C / 4])(
                            filter, nullptr,
                            static_cast<int>(reinterpret_cast<std::uintptr_t>(player)));
                }

                if (!excluded && refresh)
                {
                    if (acceptedCount >= CSF_AT(std::int32_t, self, 0x0))
                        excluded = 1;
                    else
                        ++acceptedCount;
                }
            }

            NetSpawnMessages_SetPlayerExcluded(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(player)),
                excluded);
            result = reinterpret_cast<void*>(candidateCount);
            ++index;
        }
        while (index < static_cast<std::int32_t>(candidateCount));
    }

    return result;
}

HOOK(0x00751C40, NetSpawnMessages_UpdatePlayerSelection, "auto");
