#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the first occurrence of `id` from the id list (field_0044, count field_004c),
// shifting the following entries down. Returns the new count, or the old count when
// `id` is not present.
static int __fastcall C_00862e88_RemoveIdAndCompact(C_00862e88* self, void* /*edx*/, int id)
{
    const int initialCount = static_cast<int>(self->field_004c);
    int foundIndex = -1;
    int result = 0;

    if (initialCount > 0) {
        while (foundIndex == -1) {
            auto* ids = reinterpret_cast<int*>(self->field_0044);
            if (id == ids[result]) {
                foundIndex = result;
            }
            if (++result >= initialCount) {
                if (foundIndex == -1) {
                    return result;
                }
                break;
            }
        }

        result = static_cast<int>(self->field_004c) - 1;
        self->field_004c = static_cast<uint32_t>(result);

        if (foundIndex != result) {
            auto* ids = reinterpret_cast<int*>(self->field_0044);
            return static_cast<int>(reinterpret_cast<std::intptr_t>(
                _memcpy(ids + foundIndex,
                        ids + foundIndex + 1,
                        sizeof(int) * static_cast<std::size_t>(result - foundIndex))));
        }
    }
    return result;
}

HOOK(0x006FC4B0, C_00862e88_RemoveIdAndCompact, "auto");
