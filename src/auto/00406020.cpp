#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Callee: sub_4088B0 (__fastcall, self in ecx, edx unused, one stack arg, ret 4).
CSF_FUNC(char, __fastcall, ProcessInnerRequest, 0x004088B0, (void* self, void* edx, int* a2));

// Tail-jump forwarder at 0x406020: jmp 0x4088B0 with ecx and the stack untouched, so the caller's
// this (ecx) and its single stack argument reach the callee as they are.
static char __fastcall C_0082d648_ForwardToInnerRequest(void* self, void* edx, int* a2)
{
    (void)edx;
    return ProcessInnerRequest(self, nullptr, a2);
}

HOOK(0x00406020, C_0082d648_ForwardToInnerRequest, "auto");

