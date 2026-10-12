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

CSF_FUNC(char, __fastcall, sub_416650, 0x00416650, (void* self, void* edx, float* a2, uint32_t* a3));

static char __fastcall C_0082e128_TakePendingSnapshot(C_0082e128* self, void* /*edx*/, float* a2, uint32_t* a3)
{
    // Owner check: field_0004 -> child at +0x18 -> child->vtbl[2]() must all succeed.
    bool ownerReady = false;
    if (self->field_0004 != 0) {
        void* ownerObj = reinterpret_cast<void*>(self->field_0004);
        uint32_t childValue = CSF_AT(uint32_t, ownerObj, 0x18);
        if (childValue != 0) {
            void* childObj = reinterpret_cast<void*>(childValue);
            auto queryChild = reinterpret_cast<bool(__fastcall*)(void*, void*)>(CSF_AT(void**, childObj, 0)[2]);
            ownerReady = queryChild(childObj, nullptr);
        }
    }

    char baseResult = sub_416650(self, nullptr, a2, a3);

    if (ownerReady && baseResult) {
        void* ownerObj = reinterpret_cast<void*>(self->field_0004);
        auto notifyOwner = reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*, int)>(CSF_AT(void**, ownerObj, 0)[41]);
        notifyOwner(ownerObj, nullptr, a3, 0);
    }

    if (self->field_0074) {
        std::memcpy(a3, &self->field_0010, 0x64);
        self->field_0074 = 0;
        return 1;
    }
    return baseResult;
}

HOOK(0x00417DE0, C_0082e128_TakePendingSnapshot, "auto");
