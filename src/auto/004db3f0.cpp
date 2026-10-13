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

// TCharTask::PrepareBase: asks the virtual "return zero" slot (+0x88) for a candidate
// object. If it is non-null and differs from the current child at +0x10, the old child
// gets its virtual slot +0x94 called, the candidate is adopted (copying +0x08 into it and
// linking its back-pointer at +0x0C to this task), and control is tail-jumped into the
// new child's virtual slot +0x8C.
static hook::Tail __fastcall TCharTask_PrepareBase(void* self, const uint32_t* /*args*/) {
    // First virtual call on this: slot +0x88 (34th vtable entry), no arguments.
    int candidateResult =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x88 / 4])(self, nullptr);
    uint32_t candidate = static_cast<uint32_t>(candidateResult);

    if (candidate != 0) {
        uint32_t currentChild = CSF_AT(uint32_t, self, 0x10);
        if (candidate != currentChild) {
            if (currentChild != 0) {
                void* oldChild = reinterpret_cast<void*>(currentChild);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, oldChild, 0)[0x94 / 4])(oldChild, nullptr);
            }

            uint32_t parentLink = CSF_AT(uint32_t, self, 0x08);
            CSF_AT(uint32_t, self, 0x10) = candidate;

            void* newChild = reinterpret_cast<void*>(candidate);
            CSF_AT(uint32_t, newChild, 0x08) = parentLink;
            CSF_AT(uint32_t, reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)), 0x0C) =
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));

            void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
            return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x8C / 4]);
        }
    }

    return hook::tail_ret(candidateResult);
}

HOOK_TAIL(0x004DB3F0, TCharTask_PrepareBase, "ai_char_task_forwarders");
