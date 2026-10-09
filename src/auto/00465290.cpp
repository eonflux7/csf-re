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

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FillEntityElementRange, 0x004F0210, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(int, __fastcall, DestroyElementRangeBuffer, 0x0044AFF0, (void* self, void* edx));

// Local layout of the 16-byte range block the original zeroes at [ebp-0x10].
struct EntityElementRange {
    uint32_t field_0000;
    uint32_t beginPtr_0004;
    uint32_t endPtr_0008;
    uint32_t field_000c;
};

static int __stdcall StoreElementCountOfEntity(int a1, int unusedArg1C, int* a3)
{
    // Service object lookup: GetValue8B0554() returns a root; the service sits at +0x1FDC.
    uint8_t* serviceRoot = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* serviceObj = CSF_AT(void*, serviceRoot, 0x1FDC);

    EntityElementRange range;
    memset(&range.beginPtr_0004, 0, 12);

    // entity = *(*(*a3 + 4) + 4)
    uintptr_t ownerAddr = static_cast<uintptr_t>(*a3);
    uintptr_t innerAddr = *reinterpret_cast<uint32_t*>(ownerAddr + 4);
    void* entity = reinterpret_cast<void*>(
        static_cast<uintptr_t>(*reinterpret_cast<uint32_t*>(innerAddr + 4)));

    FillEntityElementRange(serviceObj, nullptr, entity, &range);

    // Element count = (end - begin) / 12 when begin is set; signed division.
    int elementCount = 0;
    if (range.beginPtr_0004 != 0) {
        uint32_t byteSpan = range.endPtr_0008 - range.beginPtr_0004;
        elementCount = static_cast<int>(byteSpan) / 12;
    }

    float countAsFloat = static_cast<float>(elementCount);
    if (elementCount < 0) {
        countAsFloat += g_UnsignedIntegerBias;
    }

    // Store into *(a1 + 4) + 4.
    uint8_t* outHolder = *reinterpret_cast<uint8_t**>(static_cast<uintptr_t>(a1) + 4);
    *reinterpret_cast<float*>(outHolder + 4) = countAsFloat;

    return DestroyElementRangeBuffer(&range, nullptr);
}

HOOK(0x00465290, StoreElementCountOfEntity, "auto");
