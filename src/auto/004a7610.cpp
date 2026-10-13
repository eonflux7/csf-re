#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TCharTask_ForwardUpdateToChild, 0x4DB0F0, (void* self, void* edx, int a2));

// Forwards the update to the child, then if the depth counter at +0x20 is positive,
// pushes two values through the owner's child-object chain, and finally decrements the counter.
static int __fastcall TCharTask_UpdateChildrenAndDecrementDepth(void* self, void* /*edx*/, int a2)
{
    TCharTask_ForwardUpdateToChild(self, nullptr, a2);

    int result = CSF_AT(int, self, 0x20);
    if (result > 0)
    {
        // First chain: owner->[0x118] -> slot 10 -> slot 6 -> slot 15(field_0014)
        void* owner = CSF_AT(void*, self, 0x08);
        void* child = CSF_AT(void*, owner, 0x118);
        void* firstObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[10])(child, nullptr);
        void* firstSub = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, firstObj, 0)[6])(firstObj, nullptr);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, firstSub, 0)[15])(
            firstSub, nullptr, CSF_AT(int, self, 0x14));

        // Second chain: same path, slot 17 with field_001C returns the result
        owner = CSF_AT(void*, self, 0x08);
        child = CSF_AT(void*, owner, 0x118);
        void* secondObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[10])(child, nullptr);
        void* secondSub = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, secondObj, 0)[6])(secondObj, nullptr);
        result = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, secondSub, 0)[17])(
            secondSub, nullptr, CSF_AT(int, self, 0x1C));
    }

    --CSF_AT(int, self, 0x20);
    return result;
}

HOOK(0x4A7610, TCharTask_UpdateChildrenAndDecrementDepth, "auto");
