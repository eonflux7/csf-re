#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, InitializePointerStorage, 0x730B40,
         (int* self, void* edx, int count));

CSF_FUNC(std::uint32_t, __stdcall, ForwardTo730850, 0x7309B0,
         (std::uint32_t first, std::uint32_t last, std::uint32_t destination));

CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x558790,
         (int* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCurrentException, 0x7AF666,
         (void* exceptionObject, void* throwInformation));

static int* __fastcall ConstructPointerVectorCopy(
    int* self, void* /*edx*/, const int* source)
{
    const std::uint32_t initialFirst = CSF_AT(std::uint32_t, source, 0x04);
    int count = 0;
    if (initialFirst != 0)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, source, 0x08) - initialFirst;
        count = static_cast<std::int32_t>(byteCount) >> 2;
    }

    if (InitializePointerStorage(self, nullptr, count))
    {
        try
        {
            const std::uint32_t last = CSF_AT(std::uint32_t, source, 0x08);
            const std::uint32_t first = CSF_AT(std::uint32_t, source, 0x04);
            const std::uint32_t destination = CSF_AT(std::uint32_t, self, 0x04);

            CSF_AT(std::uint32_t, self, 0x08) =
                ForwardTo730850(first, last, destination);
        }
        catch (...)
        {
            DestroyPointerStorage(self, nullptr);
            RethrowCurrentException(nullptr, nullptr);
        }
    }

    return self;
}

HOOK(0x730E00, ConstructPointerVectorCopy, "auto");
