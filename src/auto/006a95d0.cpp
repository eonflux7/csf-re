#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(uint32_t*, __fastcall, sub_6508A0, 0x006508A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_799020, 0x00799020, (void* self, void* edx, void* a2, int a3, int a4, char a5));

static uint32_t* __fastcall C_0085839c_BeginGathering(C_0085839c* self, void* /*edx*/, int a2, uint32_t* a3, uint32_t* a4)
{
    uint32_t* result = a3;
    *a3 = 0xFFFFFFFFu;
    *a4 = 0xFFFFFFFFu;

    if (CSF_AT(uint32_t, self, 0x04) == 0)
    {
        CSF_AT(int, self, 0x40) = a2;
        CSF_AT(uint32_t*, self, 0x30) = a3;
        CSF_AT(uint32_t*, self, 0x34) = a4;

        void* resourceMemory = NetZ_OperatorNew(0x70);
        void* resource = nullptr;
        if (resourceMemory)
            resource = ConstructResource(resourceMemory, nullptr);
        CSF_AT(void*, self, 0x38) = resource;

        uint32_t* requestPtr = CSF_AT(uint32_t*, self, 0x40);
        result = requestPtr;
        if (requestPtr)
        {
            uint8_t* gatherBuffer = reinterpret_cast<uint8_t*>(self) + 0x18;
            sub_6508A0(gatherBuffer, nullptr);

            uint32_t* requestPtr2 = CSF_AT(uint32_t*, self, 0x40);
            uint32_t* requestInner = CSF_AT(uint32_t*, requestPtr2, 0x04);
            uint32_t requestValue = CSF_AT(uint32_t, requestInner, 0x04);

            void* resourceField = CSF_AT(void*, self, 0x38);
            void* ownerField = CSF_AT(void*, self, 0x3C);

            char callResult = sub_799020(
                ownerField,
                nullptr,
                resourceField,
                static_cast<int>(requestValue),
                static_cast<int>(reinterpret_cast<uintptr_t>(gatherBuffer)),
                1);

            result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint8_t>(callResult)));
            if (callResult)
                CSF_AT(uint32_t, self, 0x04) = 1;
        }
    }
    return result;
}

HOOK(0x006A95D0, C_0085839c_BeginGathering, "auto");
