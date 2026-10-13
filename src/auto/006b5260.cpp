#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateVectorStorage, 0x00730B40,
         (void* self, void* edx, int count));

CSF_FUNC(int, __fastcall, RebuildVectorRange_6B4DF0, 0x006B4DF0,
         (void* self, void* edx, int a1, int a2, int a3));

CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790,
         (S_0047ea40* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int* __fastcall CopyVectorRangeFromSource(
    S_006b4df0* self, void* /*edx*/, const int* source)
{
    // Compiler-emitted references (EH frame / class vtable); no runtime effect.
    (void)CSF_VTBL(0x006B4DF0);
    (void)CSF_ADDR_DATA(0x0088FA84);

    int elementCount = 0;
    const std::uint32_t initialBegin = CSF_AT(std::uint32_t, source, 0x04);
    if (initialBegin != 0)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, source, 0x08) - initialBegin;
        elementCount = static_cast<std::int32_t>(byteCount) >> 2;
    }

    if (AllocateVectorStorage(self, nullptr, elementCount))
    {
        try
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, source, 0x08);
            const std::uint32_t begin = CSF_AT(std::uint32_t, source, 0x04);
            const std::uint32_t tag = self->field_0004;

            self->field_0008 = static_cast<std::uint32_t>(
                RebuildVectorRange_6B4DF0(self, nullptr,
                                          static_cast<int>(begin),
                                          static_cast<int>(end),
                                          static_cast<int>(tag)));
        }
        catch (...)
        {
            S_0047ea40_DestroyPointerStorage(reinterpret_cast<S_0047ea40*>(self), nullptr);
            RethrowCppException(nullptr, nullptr);
        }
    }

    return reinterpret_cast<int*>(self);
}

HOOK(0x6B5260, CopyVectorRangeFromSource, "auto");
