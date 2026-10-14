#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_44BB40, 0x0044BB40, (void* self, void* edx, int a2));

static int __stdcall DispatchToDefaultDestination(int a1, int a2, int a3)
{
    // a3 points to a pointer chain: *a3 -> object, object+4 -> record, record+1 is a flag byte.
    uint8_t* object = *reinterpret_cast<uint8_t**>(static_cast<uintptr_t>(a3));
    uint8_t* record = *reinterpret_cast<uint8_t**>(object + 4);
    int flagIsZero = (record[1] == 0) ? 1 : 0;

    // Pushed before the calls below; it is the stack argument of sub_44BB40.
    int globalValue = GetGlobalValue_8B0568();
    void* globalObject = reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue));

    // The original passes the result of DefaultDestination_Get (eax) as `this` to sub_44BB40 (mov ecx, eax).
    void* destination = DefaultDestination_Get(globalObject, nullptr);
    return sub_44BB40(destination, nullptr, flagIsZero);
}

HOOK(0x0045B440, DispatchToDefaultDestination, "auto");
