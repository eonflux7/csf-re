// Reimplementation of 0x006F7540: when an object's attachment has a non-zero XY offset and
// its material effects query succeeds, configure the object's material effects and append
// an {object, x, y} record to the caller's record array.
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

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(bool, __fastcall, GfxObjectProps_HasNonZeroOffsetXY, 0x006B8B40, (void* self, void*));
CSF_FUNC(int, __cdecl, RpMatFXAtomicQueryEffects, 0x005712A0, (int a1));
CSF_FUNC(int, __cdecl, GfxFrames_ConfigureMaterialEffects, 0x006C07D0, (int object, int attachmentOwner));
CSF_FUNC(double, __fastcall, sub_6B8320, 0x006B8320, (void* self, void* edx));
CSF_FUNC(double, __fastcall, sub_6B8340, 0x006B8340, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, RecordArray_Grow_6D9EB0, 0x006D9EB0, (void* self, void* edx, int a2));

static int CollectMaterialEffectAttachment(int objectIndex, uint32_t* recordArray)
{
    int result = GfxObjectProps_GetObjectAttachment(objectIndex);
    int attachment = result;
    if (attachment == 0) {
        return result;
    }

    void* attachmentPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(attachment));
    bool hasOffset = GfxObjectProps_HasNonZeroOffsetXY(attachmentPtr, nullptr);
    result = hasOffset ? 1 : 0;
    if (!hasOffset) {
        return result;
    }

    result = RpMatFXAtomicQueryEffects(objectIndex);
    if (result == 0) {
        return result;
    }

    GfxFrames_ConfigureMaterialEffects(objectIndex, 0);
    float offsetX = static_cast<float>(sub_6B8320(attachmentPtr, nullptr));
    float offsetY = static_cast<float>(sub_6B8340(attachmentPtr, nullptr));

    if (recordArray[2] >= recordArray[1]) {
        RecordArray_Grow_6D9EB0(reinterpret_cast<void*>(recordArray), nullptr,
                                static_cast<int>(recordArray[3]));
    }

    uint32_t count = recordArray[2];
    uint8_t* record = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(recordArray[0] + 12u * count));
    recordArray[2] = count + 1;

    CSF_AT(int, record, 0) = objectIndex;
    CSF_AT(float, record, 4) = offsetX;
    CSF_AT(float, record, 8) = offsetY;

    uint32_t offsetYBits;
    std::memcpy(&offsetYBits, &offsetY, sizeof(offsetYBits));
    return static_cast<int>(offsetYBits);
}

HOOK(0x6F7540, CollectMaterialEffectAttachment, "auto");
