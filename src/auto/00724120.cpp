#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t*, __fastcall, EraseWideStringRecords, 0x007238E0, (int* self, void* , std::uint32_t* resultIterator, std::uint32_t* first, std::uint32_t* last));
CSF_FUNC(int, __fastcall, UpdateRowTextRange, 0x00723C90, (void* self, void* edx, char* text, std::uint32_t count, std::uint32_t* style));

static int __fastcall S_007238e0_ConstructRowCells(
    S_007238e0* self, void* /*edx*/,
    unsigned int requestedCount,
    int a3, int a4, int a5, int a6, int a7, int a8, int a9,
    void* block,
    int a11, int a12, int a13, int a14,
    unsigned int capacity,
    int a16, int a17)
{
    // Preserve the contiguous, addressable by-value record supplied to the callees.
    std::uint32_t arguments[16] = {
        requestedCount,
        static_cast<std::uint32_t>(a3),
        static_cast<std::uint32_t>(a4),
        static_cast<std::uint32_t>(a5),
        static_cast<std::uint32_t>(a6),
        static_cast<std::uint32_t>(a7),
        static_cast<std::uint32_t>(a8),
        static_cast<std::uint32_t>(a9),
        reinterpret_cast<std::uint32_t>(block),
        static_cast<std::uint32_t>(a11),
        static_cast<std::uint32_t>(a12),
        static_cast<std::uint32_t>(a13),
        static_cast<std::uint32_t>(a14),
        capacity,
        static_cast<std::uint32_t>(a16),
        static_cast<std::uint32_t>(a17)
    };

    // The packet also labels this callee address as a vtable reference.
    (void)CSF_VTBL(0x007238E0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t result = begin
        ? static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 60)
        : 0;

    if (result < requestedCount)
    {
        const std::int32_t currentCount = begin
            ? static_cast<std::int32_t>(self->field_0008 - begin) / 60
            : 0;
        result = static_cast<std::uint32_t>(UpdateRowTextRange(
            self, nullptr,
            reinterpret_cast<char*>(self->field_0008),
            requestedCount - static_cast<std::uint32_t>(currentCount),
            &arguments[1]));
    }
    else if (begin)
    {
        result = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 60);
        if (requestedCount < result)
        {
            result = reinterpret_cast<std::uint32_t>(EraseWideStringRecords(
                reinterpret_cast<int*>(self), nullptr, &arguments[0],
                reinterpret_cast<std::uint32_t*>(begin + 60 * requestedCount),
                reinterpret_cast<std::uint32_t*>(self->field_0008)));
        }
    }

    if (arguments[13] >= 8)
    {
        result = arguments[8];
        if (result)
            result = static_cast<std::uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(result)));
    }

    return static_cast<int>(result);
}

HOOK(0x00724120, S_007238e0_ConstructRowCells, "auto");
