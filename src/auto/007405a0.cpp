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

CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_64C980, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_64CA10, 0x0064CA10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, C_74B200_SetFlagAndReturnTrue, 0x0074B200, (uint8_t* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);

static char __fastcall NetCogibleDDL_UpdateSpawnState(void* self, void* /*edx*/, float deltaTime)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    uint8_t* flags = base - 9;
    void* statsObj = base - 0x78;

    if ((*flags & 2) != 0)
        CSF_AT(float, base, 0x40) = CSF_AT(float, base, 0x40) - deltaTime;

    int active = Sub_64B250(statsObj, nullptr);
    if ((char)active != 0)
    {
        sub_64C980(statsObj, nullptr);
        if ((*flags & 2) != 0 && CSF_AT(float, base, 0x40) <= g_fZero)
        {
            *flags = (uint8_t)(*flags & 0xFD);
            C_74B200_SetFlagAndReturnTrue(flags, nullptr);
            uint8_t* other = base - 0xC;
            *other = (uint8_t)(*other + 1);
            C_74B200_SetFlagAndReturnTrue(other, nullptr);
        }
    }
    else
    {
        sub_64CA10(statsObj, nullptr);
    }

    float timer = CSF_AT(float, base, 0x38) - deltaTime;
    CSF_AT(float, base, 0x38) = timer;
    if (timer < g_MessageBoxSizeScale)
        CSF_AT(uint32_t, base, 0x38) = 0xBF800000u;

    uint32_t* head = CSF_AT(uint32_t*, base, 0x20);
    uint32_t* node = reinterpret_cast<uint32_t*>(*head);
    if (node != head)
    {
        do
        {
            uint8_t* item = reinterpret_cast<uint8_t*>(node[2]);
            if (item && item[0x10])
            {
                uint32_t* cur = node;
                bool isHead = (node == CSF_AT(uint32_t*, base, 0x20));
                node = reinterpret_cast<uint32_t*>(*node);
                if (!isHead)
                {
                    uint32_t* next = reinterpret_cast<uint32_t*>(cur[0]);
                    uint32_t* prev = reinterpret_cast<uint32_t*>(cur[1]);
                    *prev = reinterpret_cast<uint32_t>(next);
                    next[1] = reinterpret_cast<uint32_t>(prev);
                    Mem_FreeDebug(cur);
                    CSF_AT(int, base, 0x24) = CSF_AT(int, base, 0x24) - 1;
                }
                Mem_Free(item);
            }
            else
            {
                node = reinterpret_cast<uint32_t*>(*node);
            }
        } while (node != CSF_AT(uint32_t*, base, 0x20));
    }

    char result = (char)CSF_AT(uint8_t, base, 0xA0);
    if (result)
    {
        alignas(8) uint8_t recordBytes[8];
        uint32_t* rec = MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(recordBytes));
        result = (char)MpSpawn_CommitSpawnRecord(rec, nullptr);
        CSF_AT(uint8_t, base, 0x9C) = 1;
        CSF_AT(uint8_t, base, 0xA0) = 0;
    }
    CSF_AT(uint32_t, base, 0xC) = 12;
    return result;
}

HOOK(0x007405A0, NetCogibleDDL_UpdateSpawnState, "auto");
