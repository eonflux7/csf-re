#include <cstdint>
#include "hook.h"

// __thiscall sub_69E4D0(_DWORD *this, int a2)
//
// Reads the base pointer stored at offset +0 of the object and returns the
// address of the a2-th element of an array of 8-byte entries:
//   return *this + 8 * a2;
// (Hex-Rays types the result as int; the value is an element address.)
static int __fastcall GetElementByIndex(void* self, void* /*edx*/, int index)
{
    return *reinterpret_cast<int*>(self) + 8 * index;
}

HOOK(0x0069E4D0, GetElementByIndex, "auto");
