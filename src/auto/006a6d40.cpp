#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_GlobalState, 0x008BB418);

// Slot 4 (byte 0x10): bool-returning state query on the global object.
// Slot 0 (byte 0x00): release/destroy taking a flag (1 = free the object).
static void MenuAccounts_ResetPendingEdit()
{
    if (g_GlobalState != 0)
    {
        void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(g_GlobalState));
        bool isReady = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[4])(obj, nullptr);
        if (isReady)
        {
            // The original reloads g_GlobalState after the query.
            if (g_GlobalState != 0)
            {
                void* releasedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(g_GlobalState));
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, releasedObj, 0)[0])(releasedObj, nullptr, 1);
                g_GlobalState = 0;
            }
        }
    }
}

HOOK(0x6A6D40, MenuAccounts_ResetPendingEdit, "auto");
