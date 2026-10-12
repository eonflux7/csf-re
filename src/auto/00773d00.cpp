#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: 0x773280, __cdecl. The caller pushes four stack arguments (a4, table end, a3, a3).
CSF_FUNC(char*, __cdecl, sub_773280, 0x00773280, (int a1, int a2, char* a3, char* a4));

// Original: 0x51ABD0, __fastcall-style record teardown (ecx = record, edx unused).
CSF_FUNC(int, __fastcall, Tables_ClearRecordBuffers, 0x0051ABD0, (void* self, void* edx));

static constexpr std::size_t kAnimRecordSize = 0x60; // 96 bytes per record

// Original: 0x773D00, __thiscall with 3 stack args (ret 12).
// Rebuilds the record range ending at self->field_0008 when the new end differs
// from the requested end, then stores the result through *outPtr.
static char** __fastcall AnimationDefs_RebuildTableRange(S_00773d00* self, void* /*edx*/,
                                                         char** outPtr, char* newEnd,
                                                         int rangeEnd) {
    char* current = newEnd;

    if (newEnd != reinterpret_cast<char*>(rangeEnd)) {
        char* newBase = sub_773280(rangeEnd,
                                   static_cast<int>(self->field_0008),
                                   newEnd,
                                   newEnd);

        // The original reloads the table end after the call.
        char* oldEnd = reinterpret_cast<char*>(self->field_0008);

        for (char* record = newBase; record != oldEnd; record += kAnimRecordSize) {
            Tables_ClearRecordBuffers(record, nullptr);
        }

        current = newEnd;
        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(newBase));
    }

    *outPtr = current;
    return outPtr;
}

HOOK(0x773D00, AnimationDefs_RebuildTableRange, "auto");
