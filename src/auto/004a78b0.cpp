#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TCharTask_ForwardUpdateToChild, 0x4DB0F0, (TCharTask* self, void* edx, int a2));

// Resolves the child sub-object used for the three dispatch calls:
// owner(+0x08)->dispatcher(+0x118)->vslot10()->vslot6().
static void* TCharTask_ResolveChildBranch(TCharTask* self)
{
    void* owner = CSF_AT(void*, self, 0x08);
    void* dispatcher = CSF_AT(void*, owner, 0x118);
    void* firstSub = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, dispatcher, 0)[10])(dispatcher, nullptr);
    void* branch = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, firstSub, 0)[6])(firstSub, nullptr);
    return branch;
}

static int __fastcall TCharTask_UpdateChildAndDecrementCounter(TCharTask* self, void* /*edx*/, int a2)
{
    TCharTask_ForwardUpdateToChild(self, nullptr, a2);

    int result = CSF_AT(int, self, 0x20);
    if (result > 0)
    {
        void* branch = TCharTask_ResolveChildBranch(self);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, branch, 0)[15])(branch, nullptr, CSF_AT(int, self, 0x14));

        branch = TCharTask_ResolveChildBranch(self);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, branch, 0)[16])(branch, nullptr, CSF_AT(int, self, 0x18));

        branch = TCharTask_ResolveChildBranch(self);
        result = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, branch, 0)[17])(branch, nullptr, CSF_AT(int, self, 0x1C));
    }

    --CSF_AT(int, self, 0x20);
    return result;
}

HOOK(0x4A78B0, TCharTask_UpdateChildAndDecrementCounter, "auto");
