#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TaskState_GetLinkedVec, 0x0041E3E0, (void* self, void* edx, int index));
CSF_FUNC(int, __fastcall, C_41e3f0_GetLinkedField58, 0x0041E3F0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static inline uint8_t* TaskOwner(S_00421120* self)
{
    return reinterpret_cast<uint8_t*>(self->field_0008);
}

static inline void CopyVec3(uint8_t* dst, const uint32_t* src)
{
    CSF_AT(uint32_t, dst, 0) = src[0];
    CSF_AT(uint32_t, dst, 4) = src[1];
    CSF_AT(uint32_t, dst, 8) = src[2];
}

static unsigned int __fastcall TaskState_UpdatePathPoints(S_00421120* self, void* /*edx*/)
{
    unsigned int result = reinterpret_cast<unsigned int>(TaskOwner(self));
    uint8_t* owner = TaskOwner(self);

    if (CSF_AT(uint32_t, owner, 0x1c) != 0)
        return result;

    CopyVec3(owner + 0x278, &CSF_AT(uint32_t, owner, 0x218));

    unsigned int index = static_cast<unsigned int>(
        C_41e3f0_GetLinkedField58(TaskOwner(self) + 0x29c, nullptr));
    result = index;
    if (index == 0)
        return result;

    uint32_t* vec = reinterpret_cast<uint32_t*>(
        TaskState_GetLinkedVec(TaskOwner(self) + 0x29c, nullptr, static_cast<int>(index)));
    CopyVec3(TaskOwner(self) + 0x284, vec);

    unsigned int next = index + 1;
    void* count = GetCachedObjectProvider(TaskOwner(self) + 0x29c, nullptr);
    result = reinterpret_cast<unsigned int>(count);
    if (next < result)
    {
        uint32_t* vec2 = reinterpret_cast<uint32_t*>(
            TaskState_GetLinkedVec(TaskOwner(self) + 0x29c, nullptr, static_cast<int>(next)));
        CopyVec3(TaskOwner(self) + 0x290, vec2);
        result = vec2[2];
    }
    return result;
}

HOOK(0x00421120, TaskState_UpdatePathPoints, "auto");
