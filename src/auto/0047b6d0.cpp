#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resolves the sub-object at (self->field_0008 + 0x74) and runs the sub-task on it,
// using the value stored at (self->field_0008 + 0x2E8) as its argument.
CSF_FUNC(void*, __fastcall, ResolveSubObject, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, RunSubTaskWithArg, 0x0047B340, (void* self, void* edx, int arg0));

static int __fastcall S_004223f0_ResolveAndRunSubTask(S_004223f0* self, void* /*edx*/)
{
    uint32_t objectAddr = self->field_0008;
    int taskArg = CSF_AT(int, reinterpret_cast<void*>(objectAddr), 0x2E8);
    void* subObject = ResolveSubObject(self, nullptr, static_cast<int>(objectAddr + 0x74));
    return RunSubTaskWithArg(subObject, nullptr, taskArg);
}

HOOK(0x0047B6D0, S_004223f0_ResolveAndRunSubTask, "auto");
