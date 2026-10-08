#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, FreeDebugStorage, 0x00690E60,
         (void* storage));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0,
         (int mode));
CSF_FUNC(int, __cdecl, ResetRecordRange, 0x006DEC40,
         (std::uint32_t* first, std::uint32_t* last, ...));
CSF_FUNC(int, __fastcall, AssignColumnStorage, 0x00715CD0,
         (void* self, void* edx, void* source));
CSF_FUNC(int, __fastcall, AssignAuxiliaryStorage, 0x00715E70,
         (void* self, void* edx, void* source));
CSF_FUNC(int, __fastcall, AssignRecordStorage, 0x007172A0,
         (void* self, void* edx, void* source));

static void* __fastcall CGuiListBoxBase_RebuildStorage(
    CGuiListBoxBase* self, void* /*edx*/)
{
    void* header = reinterpret_cast<void*>(self->m_pHeaderText);
    if (header != nullptr)
    {
        (reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, header, 0)[0x20 / 4]))(header, nullptr);
    }

    const std::uint32_t columnsBegin = self->m_columnsBegin;
    const std::uint32_t columnCount = columnsBegin != 0
        ? static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                self->m_columnsEnd - columnsBegin) / 12)
        : 0;

    if (columnCount != 0)
    {
        std::uint32_t offset = 0;
        std::uint32_t remaining = columnCount;
        do
        {
            void* column = CSF_AT(
                void*, reinterpret_cast<void*>(self->m_columnsBegin + offset),
                0x4);
            if (column != nullptr)
            {
                (reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, column, 0)[0x20 / 4]))(column, nullptr);
            }
            offset += 12;
            --remaining;
        }
        while (remaining != 0);
    }

    // The allocator word at index zero is deliberately left uninitialized,
    // as in the original stack temporary.
    std::uint32_t temporary[4];
    temporary[1] = 0;
    temporary[2] = 0;
    temporary[3] = 0;

    void* records = reinterpret_cast<std::uint8_t*>(self) + 0x188;
    AssignRecordStorage(temporary, nullptr, records);

    std::uint32_t* firstRecord =
        CSF_AT(std::uint32_t*, self, 0x18C);
    if (firstRecord != nullptr)
    {
        std::uint32_t* lastRecord =
            CSF_AT(std::uint32_t*, self, 0x190);
        ResetRecordRange(firstRecord, lastRecord, records, self);
        if (CSF_AT(std::uint32_t, self, 0x18C) != 0)
            FreeDebugStorage(CSF_AT(void*, self, 0x18C));
    }

    CSF_AT(std::uint32_t, self, 0x18C) = 0;
    CSF_AT(std::uint32_t, self, 0x190) = 0;
    CSF_AT(std::uint32_t, self, 0x194) = 0;
    SetAllocationMode(2);
    AssignRecordStorage(records, nullptr, temporary);
    SetAllocationMode(0);

    void* recordBlock = reinterpret_cast<void*>(temporary[1]);
    if (recordBlock != nullptr)
    {
        const std::uint32_t recordEnd = temporary[2];
        if (reinterpret_cast<std::uintptr_t>(recordBlock) != recordEnd)
        {
            std::uint32_t recordFields =
                static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(recordBlock)) + 4;
            do
            {
                void* fields = reinterpret_cast<void*>(recordFields);
                void* storage = CSF_AT(void*, fields, 0);
                if (storage != nullptr)
                {
                    FreeDebugStorage(storage);
                    recordBlock = reinterpret_cast<void*>(temporary[1]);
                }
                CSF_AT(std::uint32_t, fields, 0) = 0;
                CSF_AT(std::uint32_t, fields, 4) = 0;
                CSF_AT(std::uint32_t, fields, 8) = 0;
                recordFields += 16;
            }
            while (recordFields - 4 != recordEnd);
        }
        FreeDebugStorage(recordBlock);
    }

    temporary[1] = 0;
    temporary[2] = 0;
    temporary[3] = 0;

    void* columns = reinterpret_cast<std::uint8_t*>(self) + 0x198;
    AssignColumnStorage(temporary, nullptr, columns);
    if (self->m_columnsBegin != 0)
        FreeDebugStorage(reinterpret_cast<void*>(self->m_columnsBegin));

    self->m_columnsBegin = 0;
    self->m_columnsEnd = 0;
    CSF_AT(std::uint32_t, self, 0x1A4) = 0;
    SetAllocationMode(2);
    AssignColumnStorage(columns, nullptr, temporary);
    SetAllocationMode(0);

    if (temporary[1] != 0)
        FreeDebugStorage(reinterpret_cast<void*>(temporary[1]));

    temporary[1] = 0;
    temporary[2] = 0;
    temporary[3] = 0;

    void* auxiliary = reinterpret_cast<std::uint8_t*>(self) + 0x1A8;
    AssignAuxiliaryStorage(temporary, nullptr, auxiliary);
    if (CSF_AT(std::uint32_t, self, 0x1AC) != 0)
        FreeDebugStorage(CSF_AT(void*, self, 0x1AC));

    CSF_AT(std::uint32_t, self, 0x1AC) = 0;
    CSF_AT(std::uint32_t, self, 0x1B0) = 0;
    CSF_AT(std::uint32_t, self, 0x1B4) = 0;
    SetAllocationMode(2);
    AssignAuxiliaryStorage(auxiliary, nullptr, temporary);
    SetAllocationMode(0);

    void* finalBlock = reinterpret_cast<void*>(temporary[1]);
    if (finalBlock != nullptr)
    {
        return reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(FreeDebugStorage(finalBlock)));
    }
    return finalBlock;
}

HOOK(0x007176E0, CGuiListBoxBase_RebuildStorage, "auto");
