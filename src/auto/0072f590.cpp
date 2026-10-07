#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, CopyConstructElementArray32,
         0x0072F090,
         (std::uint32_t destination, int count, int* source,
          void* containerContext, int* sourceContext));

CSF_FUNC(std::uint32_t, __fastcall, InsertElementCopies32,
         0x0072F220,
         (void* self, void* edx, char* position,
          unsigned int count, int* source));

static void* __fastcall AppendElement32(
    void* self, void* /*edx*/, int* source)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 4);

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 8) - begin) >> 5)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 12) - begin) >> 5))
    {
        return reinterpret_cast<void*>(InsertElementCopies32(
            self, nullptr,
            reinterpret_cast<char*>(CSF_AT(std::uint32_t, self, 8)),
            1, source));
    }

    const std::uint32_t insertionAddress =
        CSF_AT(std::uint32_t, self, 8);

    void* result = CopyConstructElementArray32(
        insertionAddress, 1, source, self, source);

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 32;
    return result;
}

HOOK(0x0072F590, AppendElement32, "auto");
