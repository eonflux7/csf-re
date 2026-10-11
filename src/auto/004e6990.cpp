#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void*, int additionalCapacity));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void*, std::int32_t candidate));
CSF_FUNC(int, __fastcall, sub_495160, 0x00495160, (void* self, void* edx));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, sub_4E6750, 0x004E6750, (void* self, void* edx, uint32_t* a2, int a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e9d0_HandleLinkEvent(C_0083e9d0* self, void* /*edx*/, int a2)
{
    uint8_t* msg = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
    uint32_t* endpoints = reinterpret_cast<uint32_t*>(msg + 12);

    sub_4E6750(self, nullptr, endpoints, CSF_AT(int, msg, 8));

    uint8_t* handleData = msg + 20;
    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(handleData)));
    uint8_t* target = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(resolved));

    if (target && CSF_AT(int, target, 0x50) == 0
        && (uint8_t)IsDistinctNonSentinelValue(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)),
                                               nullptr, CSF_AT(std::int32_t, target, 0x78))) {
        ++self->field_001c;
        if (PairLink_TestEndpoints(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(endpoints)))) {
            sub_495160(target, nullptr);
        }
        --self->field_001c;
    }

    uint32_t used = self->field_0014;
    uint32_t capacity = self->field_0010;
    uint8_t* records = reinterpret_cast<uint8_t*>(self) + 0xC;
    if (used >= capacity) {
        S_00413570_GrowEventRecords(reinterpret_cast<S_00413570*>(records), nullptr, CSF_AT(int, records, 0xC));
    }

    uint32_t index = CSF_AT(uint32_t, records, 8);
    uint32_t base = CSF_AT(uint32_t, records, 0);
    CSF_AT(uint32_t, records, 8) = index + 1;
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(base + 12 * index));
    CSF_AT(uint32_t, entry, 4) = CSF_AT(uint32_t, handleData, 4);
    int result = CSF_AT(int, handleData, 8);
    CSF_AT(int, entry, 8) = result;
    return result;
}

HOOK(0x004E6990, C_0083e9d0_HandleLinkEvent, "auto");
