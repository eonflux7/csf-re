#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callback that reads the byte located at offset +2 inside the object passed as
// the second argument. Only the low byte of eax is meaningful (mov al, ...), so
// the result is a genuine char return.
static char __stdcall GetObjectByteAtOffset2(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return *reinterpret_cast<char*>(static_cast<uintptr_t>(a2) + 2);
}

HOOK(0x00745520, GetObjectByteAtOffset2, "auto");
