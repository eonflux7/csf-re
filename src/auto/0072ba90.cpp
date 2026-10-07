#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, CopyBytes, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static std::uint32_t* __fastcall EraseFourByteVectorElement(
    void* self, void* /*edx*/, std::uint32_t* result, char* element)
{
    const std::uintptr_t sourceAddress =
        reinterpret_cast<std::uintptr_t>(element) + 4u;

    // Equivalent to the original 32-bit SUB, SAR 2, SHL 2.
    const std::uint32_t byteCount =
        (CSF_AT(std::uint32_t, self, 0x08) -
         static_cast<std::uint32_t>(sourceAddress)) & 0xFFFFFFFCu;

    CopyBytes(element, reinterpret_cast<const void*>(sourceAddress), byteCount);

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, self, 0x08) - 4u;

    *result = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(element));
    return result;
}

HOOK(0x0072BA90, EraseFourByteVectorElement, "auto");
