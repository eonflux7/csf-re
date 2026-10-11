#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the child task held at +0x10: calls its vtable slot 0x94 (slot 37),
// clears its back-reference at +0x0C, then clears the pointer at +0x10.
static int __fastcall TCharTask_ReleaseCurrentTask(TCharTask* self, void* /*edx*/)
{
    int result = 0;  // original leaves eax undefined when the task pointer is null

    void* currentTask = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    if (currentTask)
    {
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, currentTask, 0)[0x94 / 4])(currentTask, nullptr);

        // Re-read the task pointer after the virtual call; the callee may have changed it.
        currentTask = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
        CSF_AT(int, currentTask, 0xC) = 0;
        CSF_AT(uint32_t, self, 0x10) = 0;
    }
    return result;
}

HOOK(0x004DB110, TCharTask_ReleaseCurrentTask, "auto");
