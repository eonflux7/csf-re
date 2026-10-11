#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateRecordStorage_410BD0, 0x00410BD0,
         (void* self, void* edx, std::uint32_t count));

CSF_FUNC(int, __fastcall, BuildRecordRange_515E60, 0x00515E60,
         (void* self, void* edx, int a1, int a2, int a3));

CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790,
         (int* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static S_00515e60* __fastcall CopyStorageRangeFromSource(
    S_00515e60* self, void* /*edx*/, const int* source)
{
    // Compiler-emitted references (EH frame / class vtable / handler data); no runtime effect.
    (void)CSF_VTBL(0x00515E60);
    (void)CSF_ADDR_DATA(0x0088B060);

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
                BuildRecordRange_515E60(self, nullptr,
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

    return self;
}

HOOK(0x516490, CopyStorageRangeFromSource, "auto");

