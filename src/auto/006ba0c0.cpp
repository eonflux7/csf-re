#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, GfxObjectProps_FindPropertyReference,
                0x006B9130, (void* self, void* edx, int a2));
static CSF_FUNC(int, __fastcall, GfxObjectProps_ReleasePropertyReference,
                0x006B9CB0, (void* self, void* edx, int* a2));

static CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
static CSF_GLOBAL(int, g_CurrentPropertyReference, 0x008BB470);
static CSF_GLOBAL(int, g_PropertyTableOffset, 0x008BB474);
static CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);

[[maybe_unused]] static const auto g_PropertyTableVtable =
    CSF_VTBL(0x006B9130);

static int __cdecl GfxObjectProps_CopyPropertyReference(
    int destinationOffset, int sourceOffset)
{
    void* engine = g_pRwEngineInstance;
    int tableOffset = g_PropertyTableOffset;
    auto* entries = static_cast<std::uint8_t*>(g_EntryTable);
    auto* table = CSF_AT(
        S_006b9130*, static_cast<std::uint8_t*>(engine) + tableOffset, 4);

    int sourceReference = CSF_AT(int, entries + sourceOffset, 4);
    int destinationReference = CSF_AT(int, entries + destinationOffset, 4);

    if (destinationReference)
    {
        int referenceToRelease = destinationReference;
        bool wasCurrent = destinationReference == g_CurrentPropertyReference;
        int remaining = GfxObjectProps_ReleasePropertyReference(
            table, nullptr, &referenceToRelease);

        if (wasCurrent && !remaining)
            g_CurrentPropertyReference = 0;

        entries = static_cast<std::uint8_t*>(g_EntryTable);
        destinationReference = 0;
    }

    if (sourceReference)
    {
        int acquiredReference = 0;
        int index = GfxObjectProps_FindPropertyReference(
            table, nullptr, sourceReference);

        if (index != -1)
        {
            std::uint32_t rowOffset = static_cast<std::uint32_t>(index) * 8;
            auto* row = reinterpret_cast<std::uint8_t*>(table->field_0004)
                        + rowOffset;
            CSF_AT(std::uint32_t, row, 4) += 1;

            acquiredReference = CSF_AT(
                int,
                reinterpret_cast<std::uint8_t*>(table->field_0004) + rowOffset,
                0);
        }

        CSF_AT(int, entries + destinationOffset, 4) = acquiredReference;
    }
    else
    {
        CSF_AT(int, entries + destinationOffset, 4) = destinationReference;
    }

    return destinationOffset;
}

HOOK(0x006BA0C0, GfxObjectProps_CopyPropertyReference, "auto");
