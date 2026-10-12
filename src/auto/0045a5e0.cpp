#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* , int key));
CSF_FUNC(int, __fastcall, InitPoseScratchBuffer, 0x004582E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObjList_FindIndex, 0x00458930, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, HudStatusIcons_RemoveByName, 0x004590D0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, HudStatusIcons_AppendRecord, 0x00459FA0, (void* self, void* edx, char* a3));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Listed as a vtable reference in the packet, although the instruction is a call.
[[maybe_unused]] static auto const g_SceneObjListPacketVtable = CSF_VTBL(0x00458930);

static void __fastcall S_00458930_UpdateStatusIcon(
    S_00458930* self, void* /*edx*/, char* record)
{
    const uint32_t statusBegin = self->field_002c;
    if (!statusBegin ||
        static_cast<int32_t>(self->field_0030 - statusBegin) / 20 == 0)
    {
        return;
    }

    const int key = CSF_AT(int, record, 0);
    CHud* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
    if (!CHud_LookupValueByKey(hud, nullptr, key))
    {
        HudStatusIcons_RemoveByName(
            self, nullptr, reinterpret_cast<int>(record + 28));
        return;
    }

    int index = SceneObjList_FindIndex(
        self, nullptr, reinterpret_cast<int>(record + 28));

    if (index == -1)
    {
        HudStatusIcons_AppendRecord(
            reinterpret_cast<uint8_t*>(self) + 8, nullptr, record);

        const uint32_t recordsBegin = self->field_000c;
        if (recordsBegin)
        {
            index = static_cast<int32_t>(
                CSF_AT(uint32_t, self, 0x10) - recordsBegin) / 40 - 1;
        }
        else
        {
            index = -1;
        }
    }
    else
    {
        const uint32_t offset = static_cast<uint32_t>(index) * 40;
        InitPoseScratchBuffer(
            reinterpret_cast<void*>(self->field_000c + offset), nullptr);

        void* destination =
            reinterpret_cast<void*>(self->field_000c + offset);
        std::memcpy(destination, record, 0x1C);
        CSF_AT(uint32_t, destination, 0x20) =
            CSF_AT(uint32_t, record, 0x20);
        CSF_AT(uint32_t, destination, 0x24) =
            CSF_AT(uint32_t, record, 0x24);
    }

    void* destination = reinterpret_cast<void*>(
        self->field_000c + static_cast<uint32_t>(index) * 40);
    float metric = CSF_AT(float, destination, 8);
    if (metric == g_fZero)
        metric = g_MissingMetricValue;
    CSF_AT(float, destination, 8) = metric;
}

HOOK(0x0045A5E0, S_00458930_UpdateStatusIcon, "auto");
