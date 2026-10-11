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

CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, sub_7451C0, 0x007451C0, (void* self, void* edx, char arg0, char a3, char a2, char a5));
CSF_FUNC(int, __fastcall, sub_73FF00, 0x0073FF00, (void* self, void* edx, uint32_t* a2));

static int __fastcall C_00858180_EnqueueDdlEventRecord(C_00858180* self, void* /*edx*/, int arg0, int a3, char a2, int a5)
{
    int globalObject = GetGlobalDword_73C020();
    int returnValue = globalObject;

    if (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(globalObject), 0x104))
    {
        if ((uint8_t)Sub_64B250(self, nullptr))
        {
            sub_7451C0(self, nullptr, static_cast<char>(arg0), static_cast<char>(a3), a2, static_cast<char>(a5));

            uint32_t recordAddress = reinterpret_cast<uint32_t>(Mem_Alloc(20));
            returnValue = static_cast<int>(recordAddress);
            if (recordAddress)
            {
                uint8_t* record = reinterpret_cast<uint8_t*>(recordAddress);
                CSF_AT(uint8_t, record, 0x10) = 0;
                CSF_AT(int, record, 0x00) = arg0;
                CSF_AT(int, record, 0x04) = a3;
                CSF_AT(uint8_t, record, 0x08) = static_cast<uint8_t>(a2);
                CSF_AT(int, record, 0x0C) = a5;

                void* queue = reinterpret_cast<uint8_t*>(self) + 0x6DC;
                returnValue = sub_73FF00(queue, nullptr, &recordAddress);
            }
        }
    }

    return returnValue;
}

HOOK(0x00741F40, C_00858180_EnqueueDdlEventRecord, "auto");
