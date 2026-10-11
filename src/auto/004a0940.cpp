#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008343a4_FindMatchingEntry, 0x004A0080, (C_008343a4* self, void* /*edx*/, void* referenceObject));
CSF_FUNC(int, __stdcall, S_004a0490_ApplyAlertIconState, 0x0049FE70, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(void, __fastcall, S_004a0490_UnlinkEntry, 0x004A0490, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, S_004a0490_CreateAlertEntry, 0x004A0810, (void* self, void* edx, int a2));

// Vtable reference listed in the packet for 0x004A0490 (no vtable store occurs in this function).
static void S_004a0490_TouchVtableRef()
{
    (void)CSF_VTBL(0x004A0490);
}

static void __fastcall S_004a0490_UpdateAlertEntry(S_004a0490* self, void* /*edx*/,
                                                   int referenceId, int iconState,
                                                   int p4, int p5, int p6)
{
    (void)S_004a0490_TouchVtableRef;

    int matchingEntry = C_008343a4_FindMatchingEntry(
        reinterpret_cast<C_008343a4*>(self), nullptr,
        reinterpret_cast<void*>(static_cast<uintptr_t>(referenceId)));

    if (matchingEntry == 0)
    {
        if (iconState == -1)
            return;
        matchingEntry = S_004a0490_CreateAlertEntry(self, nullptr, referenceId);
    }

    S_004a0490_ApplyAlertIconState(matchingEntry, iconState, p4, p5, p6);

    uint8_t* entryBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(matchingEntry));
    int v9 = CSF_AT(int, entryBytes, 0x0C);
    CSF_AT(uint8_t, entryBytes, 0x18) = 0;
    if (v9 == 0 && CSF_AT(int, entryBytes, 0x10) == -1)
        S_004a0490_UnlinkEntry(self, nullptr, reinterpret_cast<uint32_t*>(entryBytes));
}

HOOK(0x004A0940, S_004a0490_UpdateAlertEntry, "auto");
