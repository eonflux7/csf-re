#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable of S_00517800, referenced by the original (see packet). No store is
// performed here: the original only calls 0x517800 and never writes a vtable.
static const void* const s_S00517800VtableRef = CSF_VTBL(0x00517800);

// Callee: returns an int result for the object; -1 marks "no result".
CSF_FUNC(int, __fastcall, S_00517800_Query, 0x00517800, (void* self, void* edx));

// Returns 1 when the query on this object yields -1, otherwise 0.
static int __fastcall S_00517800_IsMinusOneResult(S_00517800* self, void* /*edx*/)
{
    return S_00517800_Query(self, nullptr) == -1 ? 1 : 0;
}

HOOK(0x00506E50, S_00517800_IsMinusOneResult, "auto");
