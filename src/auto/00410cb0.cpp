#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateRecordStorage_410BD0, 0x00410BD0,
         (void* self, void* edx, std::uint32_t count));

CSF_FUNC(int, __fastcall, RebuildRecordRange_4109C0, 0x004109C0,
         (void* self, void* edx, int a1, int a2, int a3));

CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790,
         (int* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int* __fastcall CopyRecordRangeFromSource(
    S_004109c0* self, void* /*edx*/, const int* source)
{
    // Compiler-emitted references (EH frame / class vtable); no runtime effect.
    (void)CSF_VTBL(0x004109C0);
    (void)CSF_ADDR_DATA(0x00886BF0);

    int recordCount = 0;
    const std::uint32_t initialBegin = CSF_AT(std::uint32_t, source, 0x04);
    if (initialBegin != 0)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, source, 0x08) - initialBegin;
        recordCount = static_cast<std::int32_t>(byteCount) >> 4;
    }

    if (AllocateRecordStorage_410BD0(self, nullptr,
                                     static_cast<std::uint32_t>(recordCount)))
    {
        try
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, source, 0x08);
            const std::uint32_t begin = CSF_AT(std::uint32_t, source, 0x04);
            const std::uint32_t tag = self->field_0004;

            self->field_0008 = static_cast<std::uint32_t>(
                RebuildRecordRange_4109C0(self, nullptr,
                                          static_cast<int>(begin),
                                          static_cast<int>(end),
                                          static_cast<int>(tag)));
        }
        catch (...)
        {
            DestroyPointerStorage(reinterpret_cast<int*>(self), nullptr);
            RethrowCppException(nullptr, nullptr);
        }
    }

    return reinterpret_cast<int*>(self);
}

HOOK(0x410CB0, CopyRecordRangeFromSource, "auto");
