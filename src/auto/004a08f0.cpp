#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008343a4_FindMatchingEntry, 0x004A0080, (C_008343a4* self, void* /*edx*/, void* referenceObject));
CSF_FUNC(int, __fastcall, sub_4A0810, 0x004A0810, (void* self, void* /*edx*/, int a2));
CSF_FUNC(int, __fastcall, sub_49FD90, 0x0049FD90, (void* self, void* /*edx*/, int a3, int* a4, int a5, int a6));
CSF_FUNC(void, __fastcall, sub_4A0490, 0x004A0490, (void* self, void* /*edx*/, uint32_t* a2));

// __thiscall void (C_008343a4* this, int a2, int a3, int a4), ret 12
static void __fastcall C_008343a4_UpdateAlertEntryForReference(C_008343a4* self, void* /*edx*/, int a2, int a3, int a4)
{
    // Entry in the alert table for a2; create one if it does not exist yet.
    uint32_t* matchingEntry = reinterpret_cast<uint32_t*>(C_008343a4_FindMatchingEntry(self, nullptr, reinterpret_cast<void*>(static_cast<intptr_t>(a2))));
    if (!matchingEntry)
    {
        // ecx at this call is whatever C_008343a4_FindMatchingEntry left in it; the
        // closest known value is the object itself.
        matchingEntry = reinterpret_cast<uint32_t*>(sub_4A0810(self, nullptr, a2));
    }

    sub_49FD90(self, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(matchingEntry)),
               reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_0008)), a3, a4);

    if (matchingEntry[3] == 0 && matchingEntry[4] == 0xFFFFFFFFu)
    {
        (void)CSF_VTBL(0x004A0490);
        sub_4A0490(self, nullptr, matchingEntry);
    }
}

HOOK(0x004A08F0, C_008343a4_UpdateAlertEntryForReference, "auto");
