#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());

// Sums the virtual count (vtable slot 3, offset +0x0C) of four team spawn-point
// source objects, in the order the original fetches and queries them.
static int __cdecl SpawnPoints_SumTeamCounts()
{
    int spawnSourceA = GetGlobalValue_8BC1E0();
    int spawnSourceB = GetGlobalValue_753A30();
    int spawnSourceC = GetValue_8B08F8();
    int spawnSourceD = GetGlobalValue_558150();

    int countB = ((int (__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(spawnSourceB), 0)[3])(
        reinterpret_cast<void*>(spawnSourceB), nullptr);
    int sumB = ((int (__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(spawnSourceC), 0)[3])(
        reinterpret_cast<void*>(spawnSourceC), nullptr) + countB;
    int sumC = ((int (__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(spawnSourceD), 0)[3])(
        reinterpret_cast<void*>(spawnSourceD), nullptr) + sumB;
    return sumC + ((int (__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(spawnSourceA), 0)[3])(
        reinterpret_cast<void*>(spawnSourceA), nullptr);
}

HOOK(0x551BA0, SpawnPoints_SumTeamCounts, "auto");
