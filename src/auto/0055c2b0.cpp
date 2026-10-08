#include <cstdint>
#include "hook.h"
#include "globals.h"

// Global-state accessors. Both are __cdecl singletons returning an object whose
// vtable is used below. The two addresses differ, so they get distinct names.
CSF_FUNC(void*, __cdecl, GetGlobalStateObjectA, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalStateObjectB, 0x006A6D70, ());

// __thiscall helper: resets a slot in the object returned by GetGlobalStateObjectA.
CSF_FUNC(int, __fastcall, ResetGlobalStateSlot, 0x0050E1E0, (void* self, void* edx));

// __thiscall helper with four stack arguments (ret 16). Submits a request code to the global state.
CSF_FUNC(unsigned int, __fastcall, SubmitGlobalStateRequest, 0x0050F790,
         (void* self, void* edx, int requestCode, int flagA, int flagB, int flagC));

// __thiscall method taking one stack argument (ret 4). Queries the global state object and
// dispatches on the query result. Sets the state field at +0x280 to 9 or 3.
static unsigned int __fastcall MenuMpAccounts_OnStateTransition(void* self, void* /*edx*/, int /*unusedArg*/)
{
    void* stateObject = GetGlobalStateObjectB();
    int queryResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateObject, 0)[7])(stateObject, nullptr);

    if (queryResult != 0)
    {
        unsigned int result = static_cast<unsigned int>(queryResult - 2);
        if (result == 0)
        {
            void* stateObjectAgain = GetGlobalStateObjectB();
            result = ((unsigned int(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObjectAgain, 0)[17])(
                stateObjectAgain, nullptr, 0);
            CSF_AT(uint32_t, self, 0x280) = 9u;
        }
        return result;
    }

    void* slotOwner = GetGlobalStateObjectA();
    ResetGlobalStateSlot(slotOwner, nullptr);

    void* requestOwner = GetGlobalStateObjectA();
    unsigned int result = SubmitGlobalStateRequest(requestOwner, nullptr, 0x41453231, 1, 1, 0);
    CSF_AT(uint32_t, self, 0x280) = 3u;
    return result;
}

HOOK(0x0055C2B0, MenuMpAccounts_OnStateTransition, "auto");
