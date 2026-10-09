#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Destroys the elements in the range [begin, end) of the array owned by 'owner'.
CSF_FUNC(int, __cdecl, sub_468AF0, 0x00468AF0, (int* a1, int* a2, int a3, int a4));
// Frees a game-heap block (debug allocation tracker).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the dynamically allocated buffer owned by this record (buffer pointer
// at +0x04, end at +0x08, extra pointer at +0x0C). Runs the range destroyer over
// the buffer, frees the storage with the game heap and clears the three fields.
static int* __fastcall MissionScript_ReleaseEntryBuffer(int* self, void* /*edx*/)
{
    int* result = CSF_AT(int*, self, 0x04);
    if (result != nullptr)
    {
        // The local passed as the fourth argument was never initialized in the
        // original, so it is passed as 0.
        sub_468AF0(result, CSF_AT(int*, self, 0x08), reinterpret_cast<int>(self), 0);

        // The range destroyer may have modified the record, so reload the field.
        result = CSF_AT(int*, self, 0x04);
        if (result != nullptr)
        {
            result = reinterpret_cast<int*>(Mem_FreeDebug(reinterpret_cast<void*>(result)));
        }
    }

    CSF_AT(int*, self, 0x04) = nullptr;
    CSF_AT(int*, self, 0x08) = nullptr;
    CSF_AT(int*, self, 0x0C) = nullptr;
    return result;
}

HOOK(0x004696C0, MissionScript_ReleaseEntryBuffer, "auto");
