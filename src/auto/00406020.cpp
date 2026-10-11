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

// Tail-jump forwarder at 0x406020. The original never reads ecx, so this is a
// plain __stdcall function that forwards its single stack argument.
static char __stdcall C_0082d648_ForwardToInnerRequest(int* a2)
{
    return ProcessInnerRequest(nullptr, nullptr, a2);
}

HOOK(0x00406020, C_0082d648_ForwardToInnerRequest, "auto");
