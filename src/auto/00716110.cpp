#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::uint32_t, g_GuiListBoxBaseVtable, 0x00863FF8);

CSF_FUNC(int, __cdecl, FreeDebugMemory, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, ReleaseEmbeddedObject, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, DestroyColumnRecords, 0x006DEC40,
         (int* begin, int* end));
CSF_FUNC(int, __fastcall, DestroyGuiControl, 0x00719B10,
         (CGuiControl* self, void* edx));

static void DeleteGuiObject(void* object)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
}

static int __fastcall CGuiListBoxBase_Destroy(
    CGuiListBoxBase* self, void* /*edx*/)
{
    const std::uint32_t embeddedOwner = self->field_0138;
    CSF_AT(void**, self, 0) =
        reinterpret_cast<void**>(&g_GuiListBoxBaseVtable);

    if (embeddedOwner != 0)
    {
        ReleaseEmbeddedObject(
            reinterpret_cast<std::uint8_t*>(embeddedOwner) + 4, nullptr);
        self->field_0138 = 0;
    }

    void* object = reinterpret_cast<void*>(self->m_pSlider);
    if (object != nullptr)
    {
        DeleteGuiObject(object);
        self->m_pSlider = 0;
    }

    object = reinterpret_cast<void*>(self->field_013c);
    if (object != nullptr)
    {
        DeleteGuiObject(object);
        self->field_013c = 0;
    }

    object = reinterpret_cast<void*>(self->field_017c);
    if (object != nullptr)
    {
        DeleteGuiObject(object);
        self->field_017c = 0;
    }

    object = reinterpret_cast<void*>(self->m_pHeaderText);
    if (object != nullptr)
    {
        DeleteGuiObject(object);
        self->m_pHeaderText = 0;
    }

    object = reinterpret_cast<void*>(self->field_0184);
    if (object != nullptr)
    {
        DeleteGuiObject(object);
        self->field_0184 = 0;
    }

    std::uint32_t index = 0;
    std::uint32_t byteOffset = 0;
    for (;; ++index, byteOffset += 12)
    {
        const std::uint32_t begin = self->m_columnsBegin;
        if (begin == 0)
            break;

        const std::int32_t byteCount =
            static_cast<std::int32_t>(self->m_columnsEnd - begin);
        const std::uint32_t count =
            static_cast<std::uint32_t>(byteCount / 12);
        if (index >= count)
            break;

        object = reinterpret_cast<void*>(CSF_AT(
            std::uint32_t,
            reinterpret_cast<void*>(self->m_columnsBegin + byteOffset), 4));
        if (object != nullptr)
        {
            DeleteGuiObject(object);
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->m_columnsBegin + byteOffset),
                   4) = 0;
        }

        object = reinterpret_cast<void*>(CSF_AT(
            std::uint32_t,
            reinterpret_cast<void*>(self->m_columnsBegin + byteOffset), 8));
        if (object != nullptr)
        {
            DeleteGuiObject(object);
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->m_columnsBegin + byteOffset),
                   8) = 0;
        }
    }

    void* allocation = reinterpret_cast<void*>(self->field_01ac);
    if (allocation != nullptr)
        FreeDebugMemory(allocation);

    self->field_01ac = 0;
    self->field_01b0 = 0;
    self->field_01b4 = 0;

    allocation = reinterpret_cast<void*>(self->m_columnsBegin);
    if (allocation != nullptr)
        FreeDebugMemory(allocation);

    auto* recordsContainer = reinterpret_cast<std::uint8_t*>(self) + 0x188;
    self->m_columnsBegin = 0;
    self->m_columnsEnd = 0;
    self->m_columnsCap = 0;

    int* recordsBegin =
        reinterpret_cast<int*>(CSF_AT(std::uint32_t, recordsContainer, 4));
    if (recordsBegin != nullptr)
    {
        DestroyColumnRecords(
            recordsBegin,
            reinterpret_cast<int*>(
                CSF_AT(std::uint32_t, recordsContainer, 8)));

        allocation = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, recordsContainer, 4));
        if (allocation != nullptr)
            FreeDebugMemory(allocation);
    }

    CSF_AT(std::uint32_t, recordsContainer, 4) = 0;
    CSF_AT(std::uint32_t, recordsContainer, 8) = 0;
    CSF_AT(std::uint32_t, recordsContainer, 12) = 0;

    return DestroyGuiControl(static_cast<CGuiControl*>(self), nullptr);
}

HOOK(0x00716110, CGuiListBoxBase_Destroy, "auto");
