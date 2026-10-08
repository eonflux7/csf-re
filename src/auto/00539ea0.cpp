#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (bound by address).
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, IsPendingIndexMatch_443230, 0x00443230, (void* self, void* edx, int selection));
CSF_FUNC(char, __fastcall, ForwardToGlobalHandler_4FCEB0, 0x004FCEB0, (void* self, void* edx, int a3));

// Consumes the pending selection stored at +0x44. If the global match check
// succeeds, clears the selection and sets the state at +0x14 to 2, then forwards
// the stack argument to the global dispatcher.
static char __fastcall MenuMain_ConsumePendingSelection(void* self, void* /*edx*/, int dispatchArg)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    int pendingSelection = CSF_AT(int, obj, 0x44);
    int* globalValue = reinterpret_cast<int*>(GetGlobalValue_4FBDE0());
    int globalSubobject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);

    if (IsPendingIndexMatch_443230(reinterpret_cast<void*>(globalSubobject), nullptr, pendingSelection))
    {
        CSF_AT(int, obj, 0x44) = -1;
        CSF_AT(int, obj, 0x14) = 2;
    }

    void* dispatcher = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    return ForwardToGlobalHandler_4FCEB0(dispatcher, nullptr, dispatchArg);
}

HOOK(0x00539EA0, MenuMain_ConsumePendingSelection, "auto");
