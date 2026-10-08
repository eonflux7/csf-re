#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global accessor (returns the root game-state object in eax).
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

// The original passes a2 in edi. Entry thunk stores it here so the C++ body
// can hand it back to the callee thunk below.
static void* g_EntryA2 = nullptr;

// Tail-jump thunk to sub_53D740 (0x53D740). Receives ecx = a1 (self), stack
// flag. Computes ecx = self-4, restores edi = saved a2, and jumps. The callee
// pops the flag (ret 4) and returns straight to the caller of this thunk.
static __declspec(naked) int __fastcall MissionInfo_CallAdvanceTail(void* self, void* edx, int flag)
{
    __asm {
        sub ecx, 4
        mov edi, dword ptr [g_EntryA2]
        mov eax, 0x53D740
        jmp eax
    }
}

// Re-evaluates the mission-info selection state and, when the selection
// matches a pending advance (state 2 with a matching delta in the event
// argument) and the attached sub-object is flagged, triggers the advance.
static int __fastcall MissionInfo_TryAdvanceInner(uint8_t* self, void* /*edx*/, void* eventArg)
{
    uint8_t* root = reinterpret_cast<uint8_t*>(GetGlobalValue_8B056C());
    uint8_t* stateBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, root, 0x1C)));

    int32_t state = CSF_AT(int32_t, stateBase, 0x1D0);

    // Default result: address of the embedded state block (v4 + 464).
    int32_t result = static_cast<int32_t>(reinterpret_cast<uintptr_t>(stateBase + 0x1D0));

    if (state == 2)
    {
        result = CSF_AT(int32_t, stateBase, 0x1D4);

        bool shouldAdvance = false;
        if (result == 0 && CSF_AT(int32_t, eventArg, 8) > 0)
        {
            shouldAdvance = true;
        }
        else if (result == 1)
        {
            result = CSF_AT(int32_t, eventArg, 8);
            if (result < 0)
            {
                shouldAdvance = true;
            }
        }

        if (shouldAdvance)
        {
            uint8_t* attached = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x10)));
            result = static_cast<int32_t>(reinterpret_cast<uintptr_t>(attached));
            if (CSF_AT(uint8_t, attached, 4) != 0)
            {
                return MissionInfo_CallAdvanceTail(self, nullptr, 1);
            }
        }
    }
    return result;
}

// Hook entry: ecx = self, edi = a2, one stack argument (ret 4). Saves edi,
// then tail-jumps to the inner body, which keeps the same stack layout and
// pops its own argument with ret 4.
static __declspec(naked) int MissionInfo_TryAdvanceOnSelection()
{
    __asm {
        mov dword ptr [g_EntryA2], edi
        mov eax, MissionInfo_TryAdvanceInner
        jmp eax
    }
}

HOOK(0x53D8B0, MissionInfo_TryAdvanceOnSelection, "auto");
