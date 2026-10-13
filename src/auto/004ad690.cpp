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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (C_008342e8* self, void* edx));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* , float a2, char a3));

// Turns a raw 32-bit handle/integer value into a pointer without changing its bits.
static void* AsPointer(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

// Linked-object handle arrives as the raw bits of the float argument.
// Returns the value of C_0082f9f8_Start on the task object found through the link.
static int __stdcall LinkedObjectStartDirectionTask(float a2)
{
    uint32_t handleBits;
    std::memcpy(&handleBits, &a2, sizeof(handleBits));
    void* handle = AsPointer(handleBits);

    // Task object: *(*(linked - 4) + 0x2C) + 0x30
    int linkedValue = GetLinkedObjectValue_4DB5C0(handle, nullptr);
    int v2 = linkedValue ? linkedValue - 4 : 0;
    uint8_t* taskOwner = CSF_AT(uint8_t*, AsPointer(static_cast<uint32_t>(v2)), 0x2c);
    C_0082f9f8* taskObject = CSF_AT(C_0082f9f8*, taskOwner, 0x30);

    // Position owner: *(*(linked - 4) + 0x2C), then its position vector
    int linkedValue2 = GetLinkedObjectValue_4DB5C0(handle, nullptr);
    int v5 = linkedValue2 ? linkedValue2 - 4 : 0;
    uint8_t* positionOwner = CSF_AT(uint8_t*, AsPointer(static_cast<uint32_t>(v5)), 0x2c);
    float* objectPosition = GetObjectPositionVector(reinterpret_cast<C_008342e8*>(positionOwner), nullptr);

    // Linked object field at +0x10
    float* linkedField = reinterpret_cast<float*>(
        AsPointer(static_cast<uint32_t>(GetLinkedObjectField10(handle, nullptr))));

    float deltaZ = linkedField[5] - objectPosition[2];
    float* fieldVec = linkedField + 3;
    float deltaY = fieldVec[1] - objectPosition[1];

    alignas(8) uint8_t vectorBytes[0xC];
    float* vec = reinterpret_cast<float*>(vectorBytes);
    vec[0] = fieldVec[0] - objectPosition[0];
    vec[1] = deltaY;
    vec[2] = deltaZ;

    double angle = S_00526d40_GetVectorAngleRadians(reinterpret_cast<S_00526d40*>(vectorBytes), nullptr);
    float angleRadians = static_cast<float>(angle);

    return C_0082f9f8_Start(taskObject, nullptr, angleRadians, 0);
}

HOOK(0x004AD690, LinkedObjectStartDirectionTask, "auto");
