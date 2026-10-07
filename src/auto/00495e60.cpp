#include <cstdint>
#include "hook.h"

// Returns a pointer to the sub-object/field located at byte offset 0x3F0 of `self`.
static char* __fastcall GetField3F0Ptr(char* self, void* /*edx*/)
{
    return self + 1008;
}

HOOK(0x00495E60, GetField3F0Ptr, "auto");
