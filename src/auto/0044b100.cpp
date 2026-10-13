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

// Erases the 12-byte element at elementPtr from a container whose end pointer is stored at +0x08.
// Fields 1 and 2 of each following element are shifted down over it (the vtable dword of each
// element is left in place), then the last element's destructor is called with flag 0,
// the end pointer is moved back by 12, and elementPtr is written to *outPtr.
// Returns outPtr.
static uint32_t* __fastcall Seq12_EraseAtAndShift(void* self, void* /*edx*/, uint32_t* outPtr, uint32_t elementPtr)
{
    uint8_t* endPtr = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));

    // Shift fields 1..2 of the following elements down over the erased element.
    uint8_t* element = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(elementPtr));
    uint8_t* dest = element + 4;
    uint8_t* src = element + 12;
    if (src != endPtr)
    {
        do
        {
            CSF_AT(uint32_t, dest, 0) = CSF_AT(uint32_t, src, 4);
            CSF_AT(uint32_t, dest, 4) = CSF_AT(uint32_t, src, 8);
            src += 12;
            dest += 12;
        } while (src != endPtr);
    }

    // Destroy the last element (old end - 12) through its vtable slot 0 with flag 0.
    uint8_t* lastElement = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8)) - 12;
    uint8_t* oldEnd = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    if (lastElement != oldEnd)
    {
        uint8_t* cursor = lastElement;
        do
        {
            void* obj = cursor;
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 0);
            cursor += 12;
        } while (cursor != oldEnd);
    }

    // Move the end pointer back by one element.
    CSF_AT(uint32_t, self, 0x8) = CSF_AT(uint32_t, self, 0x8) - 12;

    *outPtr = elementPtr;
    return outPtr;
}

HOOK(0x44B100, Seq12_EraseAtAndShift, "auto");
