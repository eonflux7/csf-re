#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs a run of 32-byte elements into raw storage.
CSF_FUNC(std::uint32_t, __cdecl, CopyConstructElementArray32,
         0x00547600,
         (std::uint32_t destination, int count, int* source,
          void* containerContext, int* sourceContext));

// Grows the storage (reallocating/copying) and inserts count copies.
CSF_FUNC(std::uint32_t, __fastcall, InsertElementCopies32,
         0x005479F0,
         (void* self, void* edx, char* position,
          unsigned int count, int* source));

static std::uint32_t __fastcall AppendElement32(
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
        return InsertElementCopies32(
            self, nullptr,
            reinterpret_cast<char*>(CSF_AT(std::uint32_t, self, 8)),
            1, source);
    }

    const std::uint32_t insertionAddress =
        CSF_AT(std::uint32_t, self, 8);

    const std::uint32_t result = CopyConstructElementArray32(
        insertionAddress, 1, source, self, source);

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 32;
    return result;
}

HOOK(0x005483C0, AppendElement32, "auto");
