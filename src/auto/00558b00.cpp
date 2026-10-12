#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateTeamStorage_5587E0, 0x005587E0,
         (void* self, void* edx, int a2));

CSF_FUNC(int, __fastcall, RebuildTeamRange_5587C0, 0x005587C0,
         (void* self, void* edx, int a1, int a2, int a3));

CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790,
         (int* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int* __fastcall CopyTeamRangeFromSource(
    S_005587c0* self, void* /*edx*/, const int* source)
{
    // Compiler-emitted references (EH frame / class vtable / EH stub data); no runtime effect.
    (void)CSF_VTBL(0x005587C0);
    (void)CSF_ADDR_DATA(0x0088C450);

    int entryCount = 0;
    const std::uint32_t initialBegin = CSF_AT(std::uint32_t, source, 0x04);
    if (initialBegin != 0)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, source, 0x08) - initialBegin;
        entryCount = static_cast<std::int32_t>(byteCount) >> 3;
    }

    if (AllocateTeamStorage_5587E0(self, nullptr, entryCount))
    {
        try
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, source, 0x08);
            const std::uint32_t begin = CSF_AT(std::uint32_t, source, 0x04);
            const std::uint32_t tag = self->field_0004;

            self->field_0008 = static_cast<std::uint32_t>(
                RebuildTeamRange_5587C0(self, nullptr,
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

HOOK(0x558B00, CopyTeamRangeFromSource, "auto");
