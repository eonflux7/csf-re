#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, InsertRecord, 0x006F89B0,
         (void* self, void* edx, char* a1, unsigned int a3, int* arg8));

static std::uint32_t* __fastcall S_006f8ce0_InsertAndGetRecordPointer(
    S_006f8ce0* self, void* /*edx*/, std::uint32_t* result,
    char* position, int* record)
{
    const std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);
        if (byteCount / 44 != 0) {
            const int offset = static_cast<int>(
                reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 44;
        }
    }

    InsertRecord(self, nullptr, position, 1u, record);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 44u;
    return result;
}

HOOK(0x006F8CE0, S_006f8ce0_InsertAndGetRecordPointer, "auto");
