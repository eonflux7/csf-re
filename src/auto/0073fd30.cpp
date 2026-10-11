#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_64C980, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_64CA10, 0x0064CA10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall NetCogibleDDL_UpdateAndFlushList(void* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* thisPtr = reinterpret_cast<uint8_t*>(self);
    void* baseObj = thisPtr - 0x70;

    if (static_cast<char>(Sub_64B250(baseObj, nullptr)))
        sub_64C980(baseObj, nullptr);
    else
        sub_64CA10(baseObj, nullptr);

    void** head = CSF_AT(void**, thisPtr, 0x20);
    void** node = static_cast<void**>(head[0]);
    if (node != head)
    {
        do
        {
            uint8_t* item = static_cast<uint8_t*>(node[2]);
            if (item && CSF_AT(uint8_t, item, 0x10))
            {
                void** cur = node;
                void** sentinel = CSF_AT(void**, thisPtr, 0x20);
                node = static_cast<void**>(cur[0]);
                if (cur != sentinel)
                {
                    void** prev = static_cast<void**>(cur[1]);
                    prev[0] = cur[0];
                    void** next = static_cast<void**>(cur[0]);
                    next[1] = cur[1];
                    Mem_FreeDebug(cur);
                    CSF_AT(int, thisPtr, 0x24) = CSF_AT(int, thisPtr, 0x24) - 1;
                }
                Mem_Free(item);
            }
            else
            {
                node = static_cast<void**>(node[0]);
            }
        } while (node != CSF_AT(void**, thisPtr, 0x20));
    }

    CSF_AT(int, thisPtr, 0x0C) = CSF_AT(uint16_t, thisPtr, -0x26);

    char result = CSF_AT(char, thisPtr, 0x8C);
    if (result)
    {
        alignas(8) uint8_t recordBytes[8];
        uint32_t* record = MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(recordBytes));
        result = static_cast<char>(MpSpawn_CommitSpawnRecord(record, nullptr));
        CSF_AT(uint8_t, thisPtr, 0x88) = 1;
        CSF_AT(uint8_t, thisPtr, 0x8C) = 0;
    }
    return result;
}

HOOK(0x0073FD30, NetCogibleDDL_UpdateAndFlushList, "auto");
