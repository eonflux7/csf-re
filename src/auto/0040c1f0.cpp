#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_0040b8e0_EraseRange, 0x0040B8E0,
         (void* self, void* edx, uint32_t* a4, uint32_t* arg4, uint32_t* arg8));
CSF_FUNC(int, __fastcall, S_0040b8e0_InsertCopies, 0x0040BD50,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// The packet also lists the erase-range callee address as a vtable reference.
[[maybe_unused]] static const auto kEraseRangeAddressTag = CSF_VTBL(0x0040B8E0);

static int __fastcall S_0040b8e0_Resize(
    S_0040b8e0* self, void* /*edx*/, unsigned int requestedCount,
    int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    // Preserve the contiguous, writable seven-dword value passed by the caller.
    uint32_t value[7] = {
        static_cast<uint32_t>(a3), static_cast<uint32_t>(a4),
        static_cast<uint32_t>(a5), static_cast<uint32_t>(a6),
        static_cast<uint32_t>(a7), static_cast<uint32_t>(a8),
        static_cast<uint32_t>(a9)
    };

    const uint32_t begin = self->field_0004;
    const unsigned int count = begin
        ? static_cast<unsigned int>(
              static_cast<int32_t>(self->field_0008 - begin) / 28)
        : 0;

    if (count >= requestedCount)
    {
        if (begin &&
            requestedCount < static_cast<unsigned int>(
                static_cast<int32_t>(self->field_0008 - begin) / 28))
        {
            S_0040b8e0_EraseRange(
                self, nullptr, &requestedCount,
                reinterpret_cast<uint32_t*>(begin + 28 * requestedCount),
                reinterpret_cast<uint32_t*>(self->field_0008));
        }
    }
    else
    {
        const int existingCount = begin
            ? static_cast<int32_t>(self->field_0008 - begin) / 28
            : 0;

        S_0040b8e0_InsertCopies(
            self, nullptr, reinterpret_cast<char*>(self->field_0008),
            requestedCount - static_cast<unsigned int>(existingCount), value);
    }

    const int result = static_cast<int32_t>(value[3]);
    if (result)
        return Mem_Free(reinterpret_cast<void*>(value[3]));
    return result;
}

HOOK(0x0040C1F0, S_0040b8e0_Resize, "auto");
