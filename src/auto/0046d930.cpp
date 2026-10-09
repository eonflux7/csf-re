#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, sub_46C710, 0x0046C710, (void* self, void* edx, int* a4, int a1, uint32_t* arg8));
CSF_FUNC(unsigned int, __fastcall, sub_46CED0, 0x0046CED0, (void* self, void* edx, int a1, unsigned int a3, int* arg8));
CSF_FUNC(int, __cdecl, sub_4714E0, 0x004714E0, (void* Block));

[[maybe_unused]] static const auto kCallee46C710Vtbl = CSF_VTBL(0x0046C710);

// Number of 8-byte elements between begin (field_0004) and end (field_0008); 0 when empty.
static unsigned int S_0046c710_ElementCount(S_0046c710* self)
{
    uint32_t beginPtr = self->field_0004;
    if (beginPtr == 0)
        return 0;
    return static_cast<unsigned int>((int)(self->field_0008 - beginPtr) >> 3);
}

static int* __fastcall S_0046c710_ResizeTo(S_0046c710* self, void* /*edx*/, unsigned int targetCount, char fillValue, void* completionObj)
{
    uint32_t beginPtr = self->field_0004;
    unsigned int currentCount = S_0046c710_ElementCount(self);
    int* result = reinterpret_cast<int*>(static_cast<intptr_t>(currentCount));

    if (currentCount >= targetCount)
    {
        if (beginPtr != 0)
        {
            unsigned int recountedSize = S_0046c710_ElementCount(self);
            result = reinterpret_cast<int*>(static_cast<intptr_t>(recountedSize));
            if (targetCount < recountedSize)
            {
                result = sub_46C710(self, nullptr,
                                    reinterpret_cast<int*>(&targetCount),
                                    (int)(beginPtr + 8u * targetCount),
                                    reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0008)));
            }
        }
    }
    else
    {
        unsigned int existingCount = beginPtr != 0 ? S_0046c710_ElementCount(self) : 0u;
        unsigned int appended = sub_46CED0(self, nullptr,
                                           (int)self->field_0008,
                                           targetCount - existingCount,
                                           reinterpret_cast<int*>(&fillValue));
        result = reinterpret_cast<int*>(static_cast<intptr_t>(appended));
    }

    if (completionObj != nullptr)
    {
        // vtable slot 0 of the completion object, called with arg 0
        auto releaseSlot0 = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, completionObj, 0)[0]);
        releaseSlot0(completionObj, nullptr, 0);
        return reinterpret_cast<int*>(static_cast<intptr_t>(sub_4714E0(completionObj)));
    }
    return result;
}

HOOK(0x0046D930, S_0046c710_ResizeTo, "auto");
