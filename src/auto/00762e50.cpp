#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"

// Original: 0x762C30, __cdecl. Builds the replacement range and returns its start.
CSF_FUNC(char*, __cdecl, sub_762C30, 0x00762C30, (int a1, int a2, char* a3));

// Original: 0x511040, __fastcall-style entry teardown (ecx = entry, edx unused).
CSF_FUNC(int, __fastcall, sub_511040, 0x00511040, (void* self, void* edx));

static constexpr std::size_t kCharEntrySize = 0x22C; // 556 bytes per entry

// Original: 0x762E50, __thiscall with 3 stack args (ret 12).
// Rebuilds the entry range ending at self->field_0008 when the new end differs
// from the requested end, then stores the result through *outPtr.
static char** __fastcall CharDefs_RebuildTableRange(int* self, void* /*edx*/,
                                                    char** outPtr, char* newEnd,
                                                    char* rangeEnd)
{
    char* current = newEnd;

    if (newEnd != rangeEnd) {
        char* newBase = sub_762C30(reinterpret_cast<int>(rangeEnd),
                                   reinterpret_cast<int>(CSF_AT(char*, self, 0x08)),
                                   newEnd);

        // The original reloads the table end after the call.
        char* oldEnd = CSF_AT(char*, self, 0x08);

        for (char* entry = newBase; entry != oldEnd; entry += kCharEntrySize) {
            sub_511040(entry, nullptr);
        }

        current = newEnd;
        CSF_AT(char*, self, 0x08) = newBase;
    }

    *outPtr = current;
    return outPtr;
}

HOOK(0x762E50, CharDefs_RebuildTableRange, "auto");
