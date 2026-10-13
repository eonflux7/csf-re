#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(int, g_NetTempIdValue, 0x008BC188);

CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_743A10, 0x00743A10, (void* self, void* edx));
CSF_FUNC(uint8_t, __fastcall, sub_7A7FB0, 0x007A7FB0, (void* self, void* edx));

// Registers the net-object entry obtained from the temp id with the callback
// subsystem. The 0xC-byte entry lives on this function's stack frame.
static void __cdecl NetCallbacks_RegisterEntryFromTempId()
{
    alignas(8) uint8_t entryBytes[0xC];
    void* entry = entryBytes;

    SpawnPoints_ApplyTempId(entry, nullptr, g_NetTempIdValue);
    bool found = sub_743A10(entry, nullptr) != 0;

    // Re-read the first dword of the entry after the lookup call.
    void* entryFirstWord = CSF_AT(void*, entryBytes, 0);
    sub_7A7FB0(found ? entryFirstWord : nullptr, nullptr);

    SpawnPoints_ReleaseEntry(entry, nullptr);
}

HOOK(0x00743FD0, NetCallbacks_RegisterEntryFromTempId, "auto");
