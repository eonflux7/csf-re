#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, CopyDwordArrayWithContext_552EF0, 0x00552EF0, (int destination, int count, int source, int context));
CSF_FUNC(std::uint32_t, __fastcall, AppendDwordCopies_555B70, 0x00555B70, (void* self, void* edx, std::uint32_t* position, std::uint32_t count, std::int32_t* value));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall C_00864508_RequestMenuByName(
    C_00864508* self, void* /*edx*/, char* name)
{
    const std::uint32_t definitionCount = self->field_0014
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0018 - self->field_0014) >> 6)
        : 0;

    std::uint32_t index = 0;
    std::uint32_t offset = 0;
    for (; index < definitionCount; ++index, offset += 64)
    {
        auto* definition = reinterpret_cast<std::uint8_t*>(self->field_0014) + offset;
        const char* definitionName = CSF_AT(std::uint32_t, definition, 0x18) < 0x10
            ? reinterpret_cast<const char*>(definition + 4)
            : CSF_AT(const char*, definition, 4);
        if (__stricmp(definitionName, name) == 0)
            break;
    }

    if (index == definitionCount)
        return 0;
    // Diagnostics (tools/dryrun.py --input): CSF_PAGE_TRACE=1 prints each page requested by name.
    if (std::getenv("CSF_PAGE_TRACE"))
        std::fprintf(stderr, "csf: page %s\n", name);

    const std::uint32_t definitionOffset = index << 6;
    const std::uint32_t menuHandle = CSF_AT(
        std::uint32_t,
        reinterpret_cast<std::uint8_t*>(self->field_0014) + definitionOffset,
        0x38);
    if (!menuHandle)
        return 0;

    const std::uint32_t pendingCount = self->field_0034
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0038 - self->field_0034) >> 3)
        : 0;

    std::uint32_t pendingIndex = 0;
    if (pendingCount)
    {
        auto* pending = reinterpret_cast<std::uint32_t*>(self->field_0034);
        auto* activeMenus = reinterpret_cast<std::uint32_t*>(self->field_0048);
        while (activeMenus[pending[0]] != menuHandle)
        {
            ++pendingIndex;
            pending += 2;
            if (pendingIndex >= pendingCount)
                break;
        }

        if (pendingIndex < pendingCount)
        {
            auto* source = reinterpret_cast<std::uint32_t*>(self->field_0034)
                + pendingIndex * 2 + 2;
            auto* end = reinterpret_cast<std::uint32_t*>(self->field_0038);
            if (source != end)
            {
                auto* destination = source - 2;
                do
                {
                    destination[0] = source[0];
                    destination[1] = source[1];
                    source += 2;
                    destination += 2;
                } while (source != end);
            }
            self->field_0038 -= 8;
        }
    }

    if (pendingIndex != pendingCount)
        return static_cast<int>(menuHandle);

    const std::uint32_t queuedCount = self->field_0024
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 0x28) - self->field_0024) >> 2)
        : 0;

    std::uint32_t queuedIndex = 0;
    if (queuedCount)
    {
        auto* queued = reinterpret_cast<std::uint32_t*>(self->field_0024);
        while (queuedIndex < queuedCount)
        {
            if (*queued == index)
                break;
            ++queuedIndex;
            ++queued;
        }
    }
    if (queuedIndex != queuedCount)
        return static_cast<int>(menuHandle);

    const std::uint32_t activeCount = self->field_0048
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_004c - self->field_0048) >> 2)
        : 0;

    std::uint32_t activeIndex = 0;
    if (activeCount)
    {
        auto* active = reinterpret_cast<std::uint32_t*>(self->field_0048);
        while (activeIndex < activeCount)
        {
            if (*active == menuHandle)
                break;
            ++activeIndex;
            ++active;
        }
    }
    if (activeIndex != activeCount)
        return static_cast<int>(menuHandle);

    std::int32_t queuedValue = static_cast<std::int32_t>(index);
    const std::uint32_t queueBegin = self->field_0024;
    const std::uint32_t queueSize = queueBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 0x28) - queueBegin) >> 2)
        : 0;

    auto* queueObject = reinterpret_cast<std::uint8_t*>(self) + 0x20;
    if (queueBegin &&
        queueSize < static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x2C) - queueBegin) >> 2))
    {
        const std::uint32_t queueEnd = CSF_AT(std::uint32_t, self, 0x28);
        CopyDwordArrayWithContext_552EF0(
            static_cast<int>(queueEnd), 1,
            reinterpret_cast<int>(&queuedValue),
            reinterpret_cast<int>(queueObject));
        CSF_AT(std::uint32_t, self, 0x28) = queueEnd + 4;
    }
    else
    {
        AppendDwordCopies_555B70(
            queueObject, nullptr,
            reinterpret_cast<std::uint32_t*>(CSF_AT(std::uint32_t, self, 0x28)),
            1u, &queuedValue);
    }

    void* menu = CSF_AT(
        void*,
        reinterpret_cast<std::uint8_t*>(self->field_0014) + definitionOffset,
        0x38);
    void* controller =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, menu, 0)[0x20 / 4])(menu, nullptr);
    if (!controller)
        return static_cast<int>(menuHandle);

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x1A4 / 4])(controller, nullptr);
    char* transitionName =
        reinterpret_cast<char* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x190 / 4])(controller, nullptr);

    const char* currentName = self->field_0094 < 0x10
        ? reinterpret_cast<const char*>(reinterpret_cast<std::uint8_t*>(self) + 0x80)
        : CSF_AT(const char*, self, 0x80);
    const char* emptyName = reinterpret_cast<const char*>(&g_byte_82E65C);

    if (std::strcmp(transitionName, currentName) == 0 ||
        std::strcmp(transitionName, emptyName) == 0)
    {
        self->field_0078 = *reinterpret_cast<const float*>(&self->field_0070);
    }
    else
    {
        self->field_0078 = self->field_0074;
    }

    auto* currentString = reinterpret_cast<std::uint8_t*>(self) + 0x7C;
    if (std::strcmp(transitionName, emptyName) == 0)
    {
        AssignStringSubstring(currentString, nullptr, currentString, 0, -1);
    }
    else
    {
        alignas(8) std::uint8_t temporaryBytes[0x30];
        C_0082d410& temporary =
            *reinterpret_cast<C_0082d410*>(temporaryBytes);
        CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, temporaryBytes, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&temporary.field_0004)[0] = 0;

        AssignStringRange_429B60(
            &temporary, nullptr, transitionName,
            static_cast<unsigned int>(std::strlen(transitionName)));
        AssignStringSubstring(currentString, nullptr, &temporary, 0, -1);

        if (CSF_AT(std::uint32_t, temporaryBytes, 0x18) >= 0x10 &&
            temporary.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(temporary.field_0004));
        }
    }

    return static_cast<int>(menuHandle);
}

HOOK(0x0071C2A0, C_00864508_RequestMenuByName, "auto");
