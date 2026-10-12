#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destroys the buffer range owned by a record (begin at +0x04, end at +0x08).
CSF_FUNC(int, __cdecl, sub_558DA0, 0x00558DA0, (int a1, int a2, int a3, int a4));
// Frees a game-heap block (debug allocation tracker).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the dynamically allocated buffer owned by this record (buffer pointer
// at +0x04, end at +0x08, extra pointer at +0x0C). Runs the range destroyer over
// the buffer, frees the storage with the game heap and clears the three fields.
static int __fastcall MpRules_ReleaseRecordBuffer(S_004482c0* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    if (result != 0)
    {
        // The fourth argument is an uninitialized stack local in the original,
        // so it is passed as 0.
        sub_558DA0(result, static_cast<int>(self->field_0008), reinterpret_cast<int>(self), 0);

        // The range destroyer may have modified the record, so reload the field.
        result = static_cast<int>(self->field_0004);
        if (result != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));
        }
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return result;
}

HOOK(0x00559070, MpRules_ReleaseRecordBuffer, "auto");
