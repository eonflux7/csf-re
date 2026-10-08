#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global state accessors (0x6A6D70 returns the state object, 0x50D190 returns the root global state).
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, Game_DispatchMenuRequest, 0x0050F790,
         (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, MenuAccounts_FallbackToList, 0x0055C9D0, (char* self, void* edx));

// Dispatches on the state object's status code (vtable slot 7):
//   0 -> reset pending state and fall back to the list view,
//   1 -> nothing further,
//   other -> reset pending state, post a notification, and set the state flag at +0x280 to 4.
static unsigned int __fastcall MenuAccounts_DispatchProfileState(void* self, void* /*edx*/, int /*unusedArg*/)
{
    void* stateObject = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    void** vtable = *reinterpret_cast<void***>(stateObject);
    int statusCode = reinterpret_cast<int (__fastcall*)(void*, void*)>(vtable[7])(stateObject, nullptr);

    if (statusCode == 0)
    {
        void* root = reinterpret_cast<void*>(GetGlobalState_50D190());
        GlobalState_ResetPending(root, nullptr);
        return static_cast<unsigned int>(MenuAccounts_FallbackToList(static_cast<char*>(self), nullptr));
    }

    unsigned int result = static_cast<unsigned int>(statusCode) - 1u;
    if (result != 0)
    {
        void* root = reinterpret_cast<void*>(GetGlobalState_50D190());
        GlobalState_ResetPending(root, nullptr);

        void* rootForNotify = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = Game_DispatchMenuRequest(rootForNotify, nullptr, 0x41453233, 1, 1, 0);

        CSF_AT(uint32_t, self, 0x280) = 4;
    }
    return result;
}

HOOK(0x0055CBD0, MenuAccounts_DispatchProfileState, "auto");
