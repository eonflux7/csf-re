#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_54D8E0, 0x0054D8E0, (void* self, void* edx, uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void*));
CSF_FUNC(char, __fastcall, AllocateTwelveByteVectorStorage, 0x00715920, (void* self, void* edx, int count));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static int __fastcall S_0054d8e0_Assign(
    S_0054d8e0* self, void* /*edx*/, int sourceAddress)
{
    const void* source = reinterpret_cast<const void*>(sourceAddress);
    const uint32_t sourceBegin = CSF_AT(uint32_t, source, 0x004);

    int count = 0;
    if (sourceBegin != 0)
    {
        const uint32_t sourceEnd = CSF_AT(uint32_t, source, 0x008);
        count = static_cast<int32_t>(sourceEnd - sourceBegin) / 12;
    }

    if (AllocateTwelveByteVectorStorage(self, nullptr, count))
    {
        try
        {
            uint32_t* end = reinterpret_cast<uint32_t*>(
                CSF_AT(uint32_t, source, 0x008));
            uint32_t* begin = reinterpret_cast<uint32_t*>(
                CSF_AT(uint32_t, source, 0x004));
            uint32_t* destination =
                reinterpret_cast<uint32_t*>(self->field_0004);

            self->field_0008 = reinterpret_cast<uint32_t>(
                sub_54D8E0(self, nullptr, begin, end, destination));
        }
        catch (...)
        {
            S_0047ea40_DestroyPointerStorage(
                reinterpret_cast<S_0047ea40*>(self), nullptr);
            RethrowCppException(nullptr, nullptr);
        }
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0054D9D0, S_0054d8e0_Assign, "auto");
