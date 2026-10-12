#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateTeamStorage_5587E0, 0x005587E0,
         (void* self, void* edx, int a2));

CSF_FUNC(uint32_t*, __fastcall, BuildTeamRange_772100, 0x00772100,
         (void* self, void* edx, uint32_t* a1, uint32_t* a2, uint32_t* a3));

CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790,
         (S_0047ea40* self, void*));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int __fastcall CopyAnimRangeFromSource(
    S_00772100* self, void* /*edx*/, const int* source)
{
    // Compiler-emitted references (EH frame / class vtable / handler info); no runtime effect.
    (void)CSF_VTBL(0x00772100);
    (void)CSF_ADDR_DATA(0x00894240);

    int recordCount = 0;
    const std::uint32_t initialBegin = CSF_AT(std::uint32_t, source, 0x04);
    if (initialBegin != 0)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, source, 0x08) - initialBegin;
        recordCount = static_cast<std::int32_t>(byteCount) >> 3;
    }

    if (AllocateTeamStorage_5587E0(self, nullptr, recordCount))
    {
        try
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, source, 0x08);
            const std::uint32_t begin = CSF_AT(std::uint32_t, source, 0x04);
            const std::uint32_t tag = self->field_0004;

            self->field_0008 = static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(
                    BuildTeamRange_772100(self, nullptr,
                                          reinterpret_cast<std::uint32_t*>(begin),
                                          reinterpret_cast<std::uint32_t*>(end),
                                          reinterpret_cast<std::uint32_t*>(tag))));
        }
        catch (...)
        {
            S_0047ea40_DestroyPointerStorage(reinterpret_cast<S_0047ea40*>(self), nullptr);
            RethrowCppException(nullptr, nullptr);
        }
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x772460, CopyAnimRangeFromSource, "auto");
