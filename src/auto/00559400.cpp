#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_004482c0_PrepareAssignment, 0x00558840, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, S_004482c0_CopyElements, 0x00559050, (void* self, void* edx, int* a1, int* a2, int* a3));
CSF_FUNC(int, __fastcall, S_004482c0_ReleaseStorage, 0x00559070, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static int __fastcall S_004482c0_Assign(
    S_004482c0* self, void* /*edx*/, int sourceAddress)
{
    const void* source = reinterpret_cast<const void*>(sourceAddress);
    const uint32_t sourceBegin = CSF_AT(uint32_t, source, 0x004);

    int count = 0;
    if (sourceBegin != 0)
    {
        const uint32_t sourceEnd = CSF_AT(uint32_t, source, 0x008);
        count = static_cast<int32_t>(sourceEnd - sourceBegin) / 20;
    }

    if (S_004482c0_PrepareAssignment(self, nullptr, count))
    {
        try
        {
            int* end = reinterpret_cast<int*>(
                CSF_AT(uint32_t, source, 0x008));
            int* begin = reinterpret_cast<int*>(
                CSF_AT(uint32_t, source, 0x004));
            int* destination = reinterpret_cast<int*>(self->field_0004);

            self->field_0008 = reinterpret_cast<uint32_t>(
                S_004482c0_CopyElements(
                    self, nullptr, begin, end, destination));
        }
        catch (...)
        {
            S_004482c0_ReleaseStorage(self, nullptr);
            RethrowCppException(nullptr, nullptr);
        }
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00559400, S_004482c0_Assign, "auto");
