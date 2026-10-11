#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_GLOBAL(uint32_t, g_NetGhostClassId, 0x008BC174);

// Checks that the object referenced by this session's stored index exposes the
// NetGhost class id. Returns 1 on success, otherwise raises a status notice and returns 0.
static char __fastcall C_00873b14_CheckNetGhostClassRegistered(C_00873b14* self, void* /*edx*/)
{
    void* vtbl = CSF_AT(void*, self, 0);
    if (vtbl == nullptr)
    {
        Game_RaiseStatusNotice(0, 0, 0xA0030004u, 0);
        return 0;
    }

    uint32_t slotValue = CSF_AT(uint32_t, vtbl, 0x2C);
    int objectIndex = static_cast<int>(slotValue >> 22);
    int foundObject = LookupObjectByIndex(objectIndex);

    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(foundObject)));
    bool supportsNetGhostClass =
        ((bool (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, target, 0)[60 / 4])(
            target, nullptr, g_NetGhostClassId);

    if (supportsNetGhostClass)
    {
        return 1;
    }

    Game_RaiseStatusNotice(0, 0, 0xE003000Cu, 0);
    return 0;
}

HOOK(0x0073C9E0, C_00873b14_CheckNetGhostClassRegistered, "auto");
