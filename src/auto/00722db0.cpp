#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, ApplyObjectValue, 0x00714750,
         (int* self, void* edx, int value));

static int __fastcall ForwardLinkedObjectAndApplyValue(
    int* self, void* /*edx*/, int objectAddress)
{
    int result = CSF_AT(int, self, 0x218);
    if (result != 0)
    {
        void* linkedObject = reinterpret_cast<void*>(result);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, linkedObject, 0)[9])(
                linkedObject, nullptr, objectAddress);

        if (objectAddress != 0)
        {
            void* object = reinterpret_cast<void*>(objectAddress);
            int value = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[6])(object, nullptr, 1);

            return ApplyObjectValue(self, nullptr, value);
        }
    }
    return result;
}

HOOK(0x00722DB0, ForwardLinkedObjectAndApplyValue, "auto");
