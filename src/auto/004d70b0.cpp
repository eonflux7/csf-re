#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Clears the pending sub-task pointer at +0x14. If it was active, it invokes the
// pending task's slot-40 virtual (cleanup), then, when the stored result at +0x18
// is zero, calls the owner's slot-35 virtual and re-looks up the task at +0x0C + 0x54.
static int __fastcall C_00834fa8_ClearPendingAndRelookup(void* self, void* /*edx*/)
{
    int result = 0;

    void* pendingTask = CSF_AT(void*, self, 0x14);
    if (pendingTask)
    {
        auto pendingCleanup = reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, pendingTask, 0)[40]);
        pendingCleanup(pendingTask, nullptr);

        result = CSF_AT(int, self, 0x18);
        CSF_AT(void*, self, 0x14) = nullptr;

        if (!result)
        {
            void* owner = CSF_AT(void*, self, 0x08);
            void* ownerSub = CSF_AT(void*, owner, 0x14);

            auto ownerNotify = reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
                CSF_AT(void**, ownerSub, 0)[35]);
            ownerNotify(ownerSub, nullptr, 0, 0, -1, 0);

            int taskOffset = CSF_AT(int, self, 0x0C) + 0x54;
            return CharTask_LookupByOffset(self, nullptr, taskOffset);
        }
    }
    return result;
}

HOOK(0x4D70B0, C_00834fa8_ClearPendingAndRelookup, "auto");
