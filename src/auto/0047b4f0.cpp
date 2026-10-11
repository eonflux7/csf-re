#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: TCharTask action dispatcher. __thiscall-style, 3 stack args (ret 12).
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));

// Starts the shoot task (action 7) on the owning character's task object.
// Chain: this->owner (+0x08) -> owner->inner (+0x14) -> inner->charTask (+0x204).
static int __fastcall TDisparar_IssueShootTaskAction(TDisparar* self, void* /*edx*/)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* inner = CSF_AT(uint8_t*, owner, 0x14);
    TCharTask* charTask = CSF_AT(TCharTask*, inner, 0x204);

    return TaskCharAction_48CFE0(charTask, nullptr, reinterpret_cast<int>(self), 7, 1);
}

HOOK(0x47B4F0, TDisparar_IssueShootTaskAction, "auto");
