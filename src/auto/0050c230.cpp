#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Destroys the elements in the range [begin, end) of the array owned by 'owner'.
CSF_FUNC(int, __cdecl, DestroyElementRange, 0x00469760,
         (int* begin, int* end, int* owner, int* context));
// Frees a game-heap block (debug allocation tracker).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Destroys the dynamically allocated element array owned by this record
// (begin/end/capacity at +0x04/+0x08/+0x0C), frees its storage with the game
// heap and then clears all three pointer fields.
static int* __fastcall MenuManager_ClearElementArray(int* self, void* /*edx*/)
{
    int* result = CSF_AT(int*, self, 0x04);
    if (result != nullptr)
    {
        DestroyElementRange(result, CSF_AT(int*, self, 0x08), self, self);

        // The range destroyer may have modified the record, so reload the field.
        result = CSF_AT(int*, self, 0x04);
        if (result != nullptr)
        {
            result = reinterpret_cast<int*>(Mem_FreeDebug(result));
        }
    }

    CSF_AT(int*, self, 0x04) = nullptr;
    CSF_AT(int*, self, 0x08) = nullptr;
    CSF_AT(int*, self, 0x0C) = nullptr;
    return result;
}

HOOK(0x0050C230, MenuManager_ClearElementArray, "auto");
