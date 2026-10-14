#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, HudAlertIcons_InitStateByType, 0x004A0A60, (void* self, void*));
CSF_FUNC(char, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, S_0078bf70_LoadRecordFields, 0x0078BF70, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, NetPerfCounters_ReserveRecords, 0x0078C360, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, NetPerfCounters_AppendRecord, 0x0078C940, (void* self, void* edx, int* a3));

CSF_GLOBAL(const char*, g_pszKey_REGISTROS, 0x008A9DA8);
CSF_GLOBAL(const char*, g_pszKey_ID_net_perf_counters, 0x008A9DAC);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_net_perf_counters, 0x008A9DF4);

static char __fastcall NetPerfCounters_LoadVersion1(
    S_0078bf70* self, void* /*edx*/, S_00692b80* node)
{
    // The packet lists these direct-call targets as vtable references.
    (void)CSF_VTBL(0x00692B80);
    (void)CSF_VTBL(0x0078BF70);

    // Keep the name buffer adjacent to the temporary record: the original
    // scans into that record if the supplied name has no terminator.
    alignas(8) uint8_t workspace[0x130];
    char* name = reinterpret_cast<char*>(workspace);

    auto* recordsNode = reinterpret_cast<S_00692b80*>(
        LookupNamedElement(reinterpret_cast<uint8_t*>(node), nullptr,
                           g_pszKey_REGISTROS));
    (*reinterpret_cast<S_00692b80**>(workspace + 0x124)) = recordsNode;
    (*reinterpret_cast<unsigned int*>(workspace + 0x128)) =
        S_00692b80_GetChildCount(recordsNode, nullptr);

    NetPerfCounters_ReserveRecords(
        reinterpret_cast<void*>(self->field_0004), nullptr,
        (*reinterpret_cast<unsigned int*>(workspace + 0x128)));

    (*reinterpret_cast<unsigned int*>(workspace + 0x12c)) = 0;
    if ((*reinterpret_cast<unsigned int*>(workspace + 0x128)) != 0)
    {
        do
        {
            auto* component = S_00692b80_GetComponent(
                recordsNode, nullptr,
                (*reinterpret_cast<unsigned int*>(workspace + 0x12c)));

            (*reinterpret_cast<unsigned int*>(workspace + 0xa0)) = 15;
            (*reinterpret_cast<int*>(workspace + 0x9c)) = 0;
            (*reinterpret_cast<uint8_t*>(workspace + 0x8c)) = 0;
            (*reinterpret_cast<unsigned int*>(workspace + 0xbc)) = 15;
            (*reinterpret_cast<int*>(workspace + 0xb8)) = 0;
            (*reinterpret_cast<uint8_t*>(workspace + 0xa8)) = 0;
            (*reinterpret_cast<int*>(workspace + 0x84)) = 0;
            HudAlertIcons_InitStateByType(workspace + 0x84, nullptr);

            void* recordContainer = reinterpret_cast<void*>(self->field_0004);
            (*reinterpret_cast<unsigned int*>(workspace + 0x120)) = 15;
            (*reinterpret_cast<int*>(workspace + 0x11c)) = 0;
            (*reinterpret_cast<uint8_t*>(workspace + 0x10c)) = 0;
            (*reinterpret_cast<int*>(workspace + 0x80)) = -1;
            NetPerfCounters_AppendRecord(
                recordContainer, nullptr,
                reinterpret_cast<int*>(workspace + 0x80));

            auto* record = CSF_AT(
                uint8_t*, reinterpret_cast<void*>(self->field_0004), 0x08) - 164;
            CSF_AT(int, record, 0x00) = ReadIntegerAttribute(
                reinterpret_cast<uint8_t*>(component), nullptr,
                const_cast<char*>(g_pszKey_ID_net_perf_counters));

            MenuExtras_ReadText(
                reinterpret_cast<uint8_t*>(component), nullptr,
                const_cast<char*>(g_pszKey_NOMBRE_net_perf_counters),
                name, 128);

            record = CSF_AT(
                uint8_t*, reinterpret_cast<void*>(self->field_0004), 0x08) - 164;
            AssignStringRange_429B60(
                record + 136, nullptr, name,
                static_cast<unsigned int>(std::strlen(name)));

            record = CSF_AT(
                uint8_t*, reinterpret_cast<void*>(self->field_0004), 0x08) - 164;
            S_0078bf70_LoadRecordFields(
                self, nullptr, component, reinterpret_cast<int>(record));

            if ((*reinterpret_cast<unsigned int*>(workspace + 0x120)) >= 0x10 &&
                (*reinterpret_cast<void**>(workspace + 0x10c)))
                Mem_FreeDebug((*reinterpret_cast<void**>(workspace + 0x10c)));
            (*reinterpret_cast<unsigned int*>(workspace + 0x120)) = 15;
            (*reinterpret_cast<int*>(workspace + 0x11c)) = 0;
            (*reinterpret_cast<uint8_t*>(workspace + 0x10c)) = 0;

            if ((*reinterpret_cast<unsigned int*>(workspace + 0xbc)) >= 0x10 &&
                (*reinterpret_cast<void**>(workspace + 0xa8)))
                Mem_FreeDebug((*reinterpret_cast<void**>(workspace + 0xa8)));
            (*reinterpret_cast<unsigned int*>(workspace + 0xbc)) = 15;
            (*reinterpret_cast<int*>(workspace + 0xb8)) = 0;
            (*reinterpret_cast<uint8_t*>(workspace + 0xa8)) = 0;

            if ((*reinterpret_cast<unsigned int*>(workspace + 0xa0)) >= 0x10 &&
                (*reinterpret_cast<void**>(workspace + 0x8c)))
                Mem_FreeDebug((*reinterpret_cast<void**>(workspace + 0x8c)));

            unsigned int nextIndex =
                (*reinterpret_cast<unsigned int*>(workspace + 0x12c)) + 1;
            bool hasNext =
                nextIndex < (*reinterpret_cast<unsigned int*>(workspace + 0x128));
            (*reinterpret_cast<unsigned int*>(workspace + 0xa0)) = 15;
            (*reinterpret_cast<int*>(workspace + 0x9c)) = 0;
            (*reinterpret_cast<uint8_t*>(workspace + 0x8c)) = 0;
            (*reinterpret_cast<unsigned int*>(workspace + 0x12c)) = nextIndex;

            if (!hasNext)
                break;
            recordsNode = (*reinterpret_cast<S_00692b80**>(workspace + 0x124));
        } while (true);
    }
    return 1;
}

HOOK(0x0078CA00, NetPerfCounters_LoadVersion1, "auto");
