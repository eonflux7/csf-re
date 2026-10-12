#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0046bea0_AssignTextureRecords, 0x006DE9B0,
         (S_0046bea0* self, void* /*edx*/, int sourceAddress));

static S_0046bea0* __cdecl TextureLoader_MoveRecordRange(int sourceBegin, int sourceEnd,
                                                         S_0046bea0* destinationBegin)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = reinterpret_cast<std::uint32_t>(destinationBegin);

    if (source == end)
        return destinationBegin;

    do
    {
        S_0046bea0_AssignTextureRecords(reinterpret_cast<S_0046bea0*>(destination),
                                        nullptr,
                                        static_cast<int>(source));
        source += 16u;
        destination += 16u;
    }
    while (source != end);

    return reinterpret_cast<S_0046bea0*>(destination);
}

HOOK(0x006DEC00, TextureLoader_MoveRecordRange, "auto");
