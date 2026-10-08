#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// std::string assign from a source string (source, startPos, count), __thiscall with 3 stack args.
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x488830, (void* self, void* edx, const void* source, uint32_t startPos, uint32_t count));

// Game debug-heap free, __cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Loads the text from the provider object at +0x34 (vtable slot 0x93) into the string member at +0x3C,
// sets state field +0x18 to 2, and frees the temporary string's heap buffer if it was allocated.
static void* __fastcall CMenuEula_LoadTextIntoNameAndSetState(void* self, void* /*edx*/)
{
    void* textProvider = CSF_AT(void*, self, 0x34);

    // Temporary std::string frame (ebp-0x1C .. ebp-0x01).
    // +0x04 = buffer/pointer word (Block), +0x14 = size, +0x18 = capacity.
    uint8_t tempFrame[0x1C];
    *reinterpret_cast<uint32_t*>(tempFrame + 0x18) = 7u;
    *reinterpret_cast<uint32_t*>(tempFrame + 0x14) = 0u;
    *reinterpret_cast<uint16_t*>(tempFrame + 0x04) = 0u;

    // Virtual call: (provider, tempFrame, 1) -> returns the string pointer.
    void* sourceString = reinterpret_cast<void* (__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, textProvider, 0)[0x93])(textProvider, nullptr, tempFrame, 1);

    // Assign into the member string at +0x3C (address of the embedded sub-object).
    void* assignResult = AssignStringRange_488830(
        static_cast<uint8_t*>(self) + 0x3C, nullptr, sourceString, 0u, 0xFFFFFFFFu);

    CSF_AT(uint32_t, self, 0x18) = 2u;

    // Free the temporary's heap buffer when capacity >= 8 (unsigned) and the pointer is non-null.
    if (*reinterpret_cast<uint32_t*>(tempFrame + 0x18) >= 8u)
    {
        uint32_t blockPointer = *reinterpret_cast<uint32_t*>(tempFrame + 0x04);
        if (blockPointer != 0u)
        {
            return reinterpret_cast<void*>(static_cast<uintptr_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(blockPointer)))));
        }
    }

    return assignResult;
}

HOOK(0x00533660, CMenuEula_LoadTextIntoNameAndSetState, "auto");
