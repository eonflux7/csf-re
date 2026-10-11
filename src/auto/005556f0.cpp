#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __stdcall, MpSpawnPoints_DrainProjectileNotifications, 0x005534D0, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void, __stdcall, MpSpawnPoints_CollectProjectiles, 0x0073F4F0, (void* a1));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

static void** __cdecl MpSpawnPoints_ProcessProjectileNotifications()
{
    // List object: allocator state, sentinel pointer, element count.
    uint32_t listState[3];
    listState[0] = 0;
    CSF_AT(void*, listState, 0x4) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(int, listState, 0x8) = 0;

    GetGlobalDword_73C020();
    MpSpawnPoints_CollectProjectiles(listState);

    void* sentinel = CSF_AT(void*, listState, 0x4);
    void* node = CSF_AT(void*, sentinel, 0x0);
    while (node != sentinel) {
        void* projectile = CSF_AT(void*, node, 0x8);
        if (projectile) {
            if (GetValueIfNotFlagged(
                    static_cast<uint8_t*>(projectile) + 0x70, nullptr)) {
                MpSpawnPoints_DrainProjectileNotifications(
                    static_cast<int>(reinterpret_cast<uintptr_t>(projectile)));
            }
            sentinel = CSF_AT(void*, listState, 0x4);
        }
        node = CSF_AT(void*, node, 0x0);
    }

    void* result = CSF_AT(void*, sentinel, 0x0);
    CSF_AT(void*, sentinel, 0x0) = sentinel;

    sentinel = CSF_AT(void*, listState, 0x4);
    CSF_AT(void*, sentinel, 0x4) = sentinel;
    sentinel = CSF_AT(void*, listState, 0x4);
    CSF_AT(int, listState, 0x8) = 0;

    while (result != sentinel) {
        void* next = CSF_AT(void*, result, 0x0);
        Mem_FreeDebug(result);
        sentinel = CSF_AT(void*, listState, 0x4);
        result = next;
    }

    if (sentinel) {
        return reinterpret_cast<void**>(
            static_cast<uintptr_t>(Mem_FreeDebug(sentinel)));
    }
    return reinterpret_cast<void**>(result);
}

HOOK(0x005556F0, MpSpawnPoints_ProcessProjectileNotifications, "auto");
