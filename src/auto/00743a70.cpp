#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_DestroyWidgetController, 0x00438280, (void* self, void*));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, NetCallbacks_UnregisterCallback, 0x00668AC0, (int a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, NetCallbacks_IsEntryValid, 0x00743A10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCallbacks_SetServiceListState, 0x007AA910, (void* self, void* edx, char a2));

using ServiceArray = void*[4];
CSF_GLOBAL(ServiceArray, g_SessionServices, 0x008BC12C);
CSF_GLOBAL(void*, g_SessionServiceManager, 0x008BC140);
CSF_GLOBAL(void*, g_SessionService144, 0x008BC144);
CSF_GLOBAL(void*, g_SessionService148, 0x008BC148);
CSF_GLOBAL(void*, g_SessionServiceMemory, 0x008BC14C);
CSF_GLOBAL(void*, g_SessionWidgetController, 0x008BC150);
CSF_GLOBAL(std::uint32_t, g_SessionState154, 0x008BC154);
CSF_GLOBAL(std::uint32_t, g_SessionState158, 0x008BC158);
CSF_GLOBAL(int, g_SessionEntryId, 0x008BC188);

static void __cdecl NetCallbacks_StopSessionServices()
{
    NetCallbacks_UnregisterCallback(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(CSF_ADDR_FN(0x007437E0))));

    if (void* controller = g_SessionWidgetController)
    {
        Hud_DestroyWidgetController(controller, nullptr);
        Mem_Free(controller);
        g_SessionWidgetController = nullptr;
    }

    if (void* service = g_SessionService144)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, service, 0)[0])(service, nullptr, 1);
        g_SessionService144 = nullptr;
    }

    if (void* service = g_SessionService148)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, service, 0)[0])(service, nullptr, 1);
        g_SessionService148 = nullptr;
    }

    alignas(8) std::uint8_t entryBytes[0xC];
    SpawnPoints_ApplyTempId(entryBytes, nullptr, g_SessionEntryId);

    if (NetCallbacks_IsEntryValid(entryBytes, nullptr))
    {
        const char valid = NetCallbacks_IsEntryValid(entryBytes, nullptr);
        void* entryObject = CSF_AT(void*, entryBytes, 0);
        void* resolvedEntry = valid ? entryObject : nullptr;

        if (CSF_AT(std::uint32_t, resolvedEntry, 0x68))
        {
            const char stillValid = NetCallbacks_IsEntryValid(entryBytes, nullptr);
            entryObject = CSF_AT(void*, entryBytes, 0);
            resolvedEntry = stillValid ? entryObject : nullptr;
            CSF_AT(std::uint32_t, resolvedEntry, 0x68) = 0;
        }
    }

    void** serviceSlot = g_SessionServices;
    do
    {
        if (void* service = *serviceSlot)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, service, 0)[0])(service, nullptr, 1);
            *serviceSlot = nullptr;
        }
        ++serviceSlot;
    }
    while (static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(serviceSlot)) <
           static_cast<std::int32_t>(
               reinterpret_cast<std::uintptr_t>(CSF_ADDR_DATA(0x008BC13C))));

    if (void* manager = g_SessionServiceManager)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, manager, 0)[0x14 / 4])(manager, nullptr, 1);
        g_SessionServiceManager = nullptr;
    }

    if (void* memory = g_SessionServiceMemory)
    {
        Mem_Free(memory);
        g_SessionServiceMemory = nullptr;
    }

    g_SessionState154 = 0;
    g_SessionState158 = 0;
    NetCallbacks_SetServiceListState(
        reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC294)), nullptr, 0);
    SpawnPoints_ReleaseEntry(entryBytes, nullptr);
}

HOOK(0x00743A70, NetCallbacks_StopSessionServices, "auto");
