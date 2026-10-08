#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CopyConstructElementArray36, 0x00727540,
         (int destination, int count, std::uint32_t* source));
CSF_FUNC(std::uint32_t*, __fastcall, InsertElement36, 0x00727F90,
         (void* self, void* edx, std::uint32_t* result,
          char* position, int* element));

static std::uint32_t* __fastcall AppendElement36(
    void* self, void* /*edx*/, int* element)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x08) - begin) / 36);
    }

    if (begin != 0)
    {
        const std::uint32_t capacity = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x0C) - begin) / 36);

        if (size < capacity)
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);
            const int result = CopyConstructElementArray36(
                static_cast<std::int32_t>(end), 1,
                reinterpret_cast<std::uint32_t*>(element));

            CSF_AT(std::uint32_t, self, 0x08) = end + 36u;
            return reinterpret_cast<std::uint32_t*>(
                static_cast<std::uint32_t>(result));
        }
    }

    char* position = reinterpret_cast<char*>(
        CSF_AT(std::uint32_t, self, 0x08));
    return InsertElement36(
        self, nullptr, reinterpret_cast<std::uint32_t*>(&element),
        position, element);
}

HOOK(0x00728160, AppendElement36, "auto");
