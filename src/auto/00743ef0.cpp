#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MenuMpPause_PrepareInitialization, 0x007438A0, (CMenuMpPause* self, void* edx));
CSF_FUNC(char, __fastcall, C_00873b14_CheckRegisteredNetObject, 0x00743A10, (void* self, void* edx));
CSF_FUNC(uint8_t, __fastcall, NetCallbacks_ProcessReferencedObject, 0x007A7FB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x007A8ED0, (int a2));

CSF_GLOBAL(void*, g_NetPrimary, 0x008BC144);
CSF_GLOBAL(void*, g_NetSecondary, 0x008BC148);
CSF_GLOBAL(void*, g_NetPendingObject, 0x008BC14C);
CSF_GLOBAL(float, g_NetElapsedTime, 0x008BC158);
CSF_GLOBAL(int, g_NetTemporaryId, 0x008BC188);

static void __cdecl NetCallbacks_Update(float elapsedTime)
{
    alignas(8) uint8_t entryBytes[0xC];

    if (g_NetPrimary &&
        ((bool (__fastcall*)(void*, void*))CSF_AT(void**, g_NetPrimary, 0)[12 / 4])(
            g_NetPrimary, nullptr) &&
        g_NetSecondary &&
        ((bool (__fastcall*)(void*, void*))CSF_AT(void**, g_NetSecondary, 0)[12 / 4])(
            g_NetSecondary, nullptr) &&
        g_NetPendingObject &&
        !((int (__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, g_NetSecondary, 0)[16 / 4])(
                g_NetSecondary, nullptr, g_NetPendingObject, 0))
    {
        ((void (__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, g_NetPrimary, 0)[16 / 4])(
                g_NetPrimary, nullptr, g_NetPendingObject, 0);
    }

    SpawnPoints_ApplyTempId(entryBytes, nullptr, g_NetTemporaryId);
    char validEntry = C_00873b14_CheckRegisteredNetObject(entryBytes, nullptr);
    NetCallbacks_ProcessReferencedObject(
        validEntry != 0 ? CSF_AT(void*, entryBytes, 0) : nullptr, nullptr);

    if (g_NetPrimary &&
        ((bool (__fastcall*)(void*, void*))CSF_AT(void**, g_NetPrimary, 0)[12 / 4])(
            g_NetPrimary, nullptr) &&
        g_NetSecondary &&
        ((bool (__fastcall*)(void*, void*))CSF_AT(void**, g_NetSecondary, 0)[12 / 4])(
            g_NetSecondary, nullptr))
    {
        g_NetElapsedTime += elapsedTime;

        int elapsedBits;
        std::memcpy(&elapsedBits, &g_NetElapsedTime, sizeof(elapsedBits));
        if (static_cast<uint8_t>(Sub_7A8ED0(elapsedBits)) != 0)
        {
            // This callee does not use its nominal this argument.
            MenuMpPause_PrepareInitialization(nullptr, nullptr);
        }
    }

    SpawnPoints_ReleaseEntry(entryBytes, nullptr);
}

HOOK(0x00743EF0, NetCallbacks_Update, "auto");
