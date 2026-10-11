#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint8_t, g_NetworkUpdateRequested, 0x008A8925);
CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectLookup, 0x008B9A90);
CSF_GLOBAL(uint8_t, g_OnlineMenuActive, 0x008BBF98);

CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());
CSF_FUNC(int, __fastcall, Sub_647AA0, 0x00647AA0, (void* self, void* edx, int a, int b));
CSF_FUNC(char, __fastcall, Sub_655520, 0x00655520, (void* self, void* edx, int a, int b));
CSF_FUNC(int, __fastcall, NetSession_CompleteConnection, 0x006575D0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, Sub_6687B0, 0x006687B0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, NetSession_ResetOnlineMenu, 0x0073C850, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_SelectOnlineEntry, 0x0073CB80, (void* self, void* edx, void** a2));
CSF_FUNC(bool, __stdcall, NetSession_HasMatchingEntry, 0x0073D2A0, (uint32_t listHeadRef, uint32_t wantedKey));
CSF_FUNC(void, __fastcall, NetSession_RemoveWorldObjectByReference, 0x0073D410, (void* self, void* edx, void** objectReference));

static char __fastcall C_0085839c_UpdateNetworkManager(
    C_0085839c* self, void* /*edx*/, int /*unused*/)
{
    char result = static_cast<char>(g_NetworkUpdateRequested);
    if (g_NetworkUpdateRequested)
    {
        int state = CSF_AT(int, self, 0x98);
        result = static_cast<char>(state);
        if (state)
        {
            int lookupKey = ReturnZero();
            void* entry = reinterpret_cast<void*>(
                Sub_647AA0(&g_MenuMpCharacterSelectLookup, nullptr, 4, lookupKey));

            void* manager = nullptr;
            if (entry)
            {
                void* owner = CSF_AT(void*, entry, 0xC);
                if (owner)
                    manager = CSF_AT(void*, owner, 8);
            }
            result = Sub_655520(manager, nullptr, -1, 1);
        }
    }

    if (CSF_AT(int, self, 0x98) != 5)
        return result;

    alignas(8) uint8_t connectionBytes[0xC];
    alignas(8) uint8_t statusBytes[0x8];
    statusBytes[7] = 0;

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    void* operation = CSF_AT(void*, self, 0xD0);
    if (operation && CSF_AT(int, operation, 0xC) != 1)
    {
        if (NetSession_HasMatchingEntry(
                reinterpret_cast<uint32_t>(bytes + 0xAC),
                reinterpret_cast<uint32_t>(bytes + 0x160)))
        {
            NetSession_SelectOnlineEntry(
                self, nullptr, reinterpret_cast<void**>(bytes + 0x160));
        }
        else
        {
            statusBytes[7] = 1;
        }

        operation = CSF_AT(void*, self, 0xD0);
        if (operation)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, operation, 0)[0])(operation, nullptr, 1);
            CSF_AT(void*, self, 0xD0) = nullptr;
        }
        Sub_6687B0(bytes + 0xAC, nullptr);
    }
    else
    {
        operation = CSF_AT(void*, self, 0xCC);
        if (!operation || CSF_AT(int, operation, 0xC) == 1)
            return static_cast<char>(reinterpret_cast<uintptr_t>(operation));

        CSF_AT(uint32_t, connectionBytes, 0) =
            CSF_AT(uint32_t, operation, 0x44);
        CSF_AT(uint32_t, connectionBytes, 4) =
            CSF_AT(uint32_t, operation, 0x48);
        CSF_AT(uint32_t, connectionBytes, 8) =
            CSF_AT(uint32_t, operation, 0x4C);
        CSF_AT(uint32_t, statusBytes, 0) = 1;

        if (static_cast<uint8_t>(NetSession_CompleteConnection(
                connectionBytes, nullptr,
                reinterpret_cast<uint32_t*>(statusBytes))))
        {
            CSF_AT(uint8_t, self, 0xFC) = 0;
            CSF_AT(int, self, 0x98) = 6;
        }
        else
        {
            statusBytes[7] = 1;
        }

        operation = CSF_AT(void*, self, 0xCC);
        if (operation)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, operation, 0)[0])(operation, nullptr, 1);
            CSF_AT(void*, self, 0xCC) = nullptr;
        }
    }

    result = static_cast<char>(statusBytes[7]);
    if (statusBytes[7])
    {
        NetSession_RemoveWorldObjectByReference(
            self, nullptr, reinterpret_cast<void**>(bytes + 0x160));
        result = static_cast<char>(NetSession_ResetOnlineMenu(self, nullptr));
        CSF_AT(int, self, 0x98) = 2;
        g_OnlineMenuActive = 0;
    }
    return result;
}

HOOK(0x0073D550, C_0085839c_UpdateNetworkManager, "auto");
