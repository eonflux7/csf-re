#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(char, __fastcall, Global8afc78_SetEnabled, 0x0042DFF0, (void* self, void* edx, int notifyValue));
CSF_FUNC(char, __fastcall, IsPendingIndexMatch_443230, 0x00443230, (void* self, void* edx, int conditionArg));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));

// If the pending flag at +0x27C is set and the latched marker at +0x278 is still zero,
// test the pending condition; on success, latch the marker, clear the pending flag,
// and notify the global notifier. Returns the low byte of the last value in eax.
static char __fastcall CMenuControls_TryLatchPendingFlag(void* self, void* /*edx*/)
{
    uint8_t pendingFlag = CSF_AT(uint8_t, self, 0x27C);
    // `mov al, [esi+0x27c]`: al is the flag byte, the rest of eax is left as it was.
    char result = static_cast<char>(pendingFlag);

    if (pendingFlag != 0)
    {
        // `mov eax, [esi+0x278]; test eax, eax; jne exit`: eax is the latch dword on this path, and
        // a non-zero latch leaves it as the return value (not the flag byte).
        uint32_t latch = CSF_AT(uint32_t, self, 0x278);
        result = static_cast<char>(latch);

        if (latch == 0)
        {
            // Original pushes [this+0x144] before the calls below and it is consumed
            // as the argument of IsPendingIndexMatch_443230; read it first, in the same order.
            int conditionArg = CSF_AT(int, self, 0x144);

            int globalValue = GetGlobalValue_4FBDE0();
            void* ownerObject = GetGlobalSubObject_4FC0A0(reinterpret_cast<void*>(globalValue), nullptr);

            result = IsPendingIndexMatch_443230(ownerObject, nullptr, conditionArg);
            if (result != 0)
            {
                CSF_AT(uint8_t, self, 0x27C) = 0;
                CSF_AT(uint32_t, self, 0x278) = 1;

                int* notifier = reinterpret_cast<int*>(GetGlobal8afc78());
                result = Global8afc78_SetEnabled(notifier, nullptr, 1);
            }
        }
    }

    return result;
}

HOOK(0x00543B40, CMenuControls_TryLatchPendingFlag, "auto");
