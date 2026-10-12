#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees, bound by address with the signatures from the packet.
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveSkinObjectFromArg, 0x00681FC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

// Converts a 32-bit game value (returned in eax) into an object pointer.
static void* DynamicObj_AsPointer(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

// Plain function (no this): the object is passed as the stdcall argument a1.
// Pops 4 bytes (ret 4).
static int __stdcall DynamicObj_ApplyBindingThenRelease(int a1)
{
    void* obj = DynamicObj_AsPointer(a1);

    if (CSF_AT(int, obj, 0x24))
    {
        int mode = CSF_AT(int, obj, 4);
        if (mode == 0 || (mode > 2 && mode <= 6))
        {
            void* firstElement = nullptr;
            if (mode != 0)
            {
                int globalValue = GetGlobalValue_4FBDE0();
                firstElement = DynamicObj_AsPointer(GetFirstElement(DynamicObj_AsPointer(globalValue), nullptr));
            }
            else
            {
                int globalValue = GetGlobalValue_4FBDE0();
                firstElement = DynamicObj_AsPointer(ResolveSkinObjectFromArg(DynamicObj_AsPointer(globalValue), nullptr));
            }

            if (firstElement)
            {
                void* lookup = DynamicObj_AsPointer(ResolveEntryLookupValue(firstElement, nullptr));
                if (lookup)
                {
                    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, lookup, 0)[15])(
                        lookup, nullptr, CSF_AT(int, obj, 0x24));
                }
            }
        }

        // LABEL_10: forward the value to the RW manager, then run the vtable slot 0 path.
        int globalInt = GetGlobalInt();
        void* rwManager = DynamicObj_AsPointer(ResolveRwManager(globalInt, nullptr));
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, rwManager, 0)[9])(
            rwManager, nullptr, CSF_AT(int, obj, 0x24));
    }

    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 1);
}

HOOK(0x00442DF0, DynamicObj_ApplyBindingThenRelease, "auto");
