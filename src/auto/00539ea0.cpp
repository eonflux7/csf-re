#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (bound by address).
CSF_FUNC(int, __cdecl, MenuMain_GetGlobalValue, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, MenuMain_GetGlobalSubobject, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, MenuMain_IsPendingSelectionMatch, 0x00443230, (void* self, void* edx, int selection));
CSF_FUNC(char, __fastcall, MenuMain_ForwardToGlobalDispatcher, 0x004FCEB0, (void* self, void* edx, int a3));

// Consumes the pending selection stored at +0x44. If the global match check
// succeeds, clears the selection and sets the state at +0x14 to 2, then forwards
// the stack argument to the global dispatcher.
static char __fastcall MenuMain_ConsumePendingSelection(void* self, void* /*edx*/, int dispatchArg)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    int pendingSelection = CSF_AT(int, obj, 0x44);
    int* globalValue = reinterpret_cast<int*>(MenuMain_GetGlobalValue());
    int globalSubobject = MenuMain_GetGlobalSubobject(globalValue, nullptr);

    if (MenuMain_IsPendingSelectionMatch(reinterpret_cast<void*>(globalSubobject), nullptr, pendingSelection))
    {
        CSF_AT(int, obj, 0x44) = -1;
        CSF_AT(int, obj, 0x14) = 2;
    }

    void* dispatcher = reinterpret_cast<void*>(MenuMain_GetGlobalValue());
    return MenuMain_ForwardToGlobalDispatcher(dispatcher, nullptr, dispatchArg);
}

HOOK(0x00539EA0, MenuMain_ConsumePendingSelection, "auto");
