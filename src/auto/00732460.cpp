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

// Callee bindings (one name and signature per address).
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendSlotOrGrow, 0x0047EE50, (void* self, void* edx, uint32_t* pointerArg));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_PrepareEntry, 0x007324C0, (void* self, void* edx, uint32_t* a3));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Re-queues the entry when the time is non-negative and the stored list is empty
// or its last element is smaller than the time.
static void __fastcall C_0086709c_ScheduleNonNegativeTime(C_0086709c* self, void* /*edx*/, uint32_t* a2, float a3)
{
    if (a3 >= g_fZero)
    {
        uint32_t listBegin = self->field_001c;
        uint8_t* listHead = reinterpret_cast<uint8_t*>(self) + 0x18;

        if (!listBegin || !(static_cast<int>(self->field_0020 - listBegin) >> 2) ||
            *reinterpret_cast<float*>(self->field_0020 - 4) < a3)
        {
            AiAttackTasks_PrepareEntry(reinterpret_cast<uint8_t*>(self) + 8, nullptr, a2);
            AiAttackTasks_AppendSlotOrGrow(listHead, nullptr, reinterpret_cast<uint32_t*>(&a3));
        }
    }
}

HOOK(0x732460, C_0086709c_ScheduleNonNegativeTime, "auto");
