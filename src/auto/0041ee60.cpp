#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TaskState_Refresh, 0x0041E750, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, TaskState_CopyList, 0x0041E890, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, TaskState_ProcessEntry, 0x0041ED90, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, TaskState_Notify, 0x006AF6E0, (void* self, void* edx, int a2));

static int __fastcall TaskState_SetTime(S_0041e750* self, void* /*edx*/, float a2)
{
    if (self->field_0000 && self->field_0018 != a2)
    {
        uint32_t buf[4];
        uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0014));
        int index = CSF_AT(int, owner, 0x58);

        TaskState_CopyList(buf, nullptr, &self->field_0004);
        self->field_000c = 0;
        TaskState_ProcessEntry(self, nullptr,
            reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0014)) + 0x5C));

        uint32_t next = static_cast<uint32_t>(index + 1);
        uint32_t count = buf[2];
        if (next < count)
        {
            uint32_t* entry = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(buf[0]) + 12 * next);
            uint32_t remaining = count - next;
            do
            {
                TaskState_ProcessEntry(self, nullptr, entry);
                entry += 3;
                --remaining;
            } while (remaining);
        }
        TaskState_Refresh(self, nullptr);
        if (buf[0])
            Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(buf[0])));
    }

    void* target = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0014)) + 4;
    self->field_0000 = 1;
    self->field_0018 = a2;
    int bits;
    std::memcpy(&bits, &a2, sizeof(bits));
    return TaskState_Notify(target, nullptr, bits);
}

HOOK(0x0041EE60, TaskState_SetTime, "auto");
