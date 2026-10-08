#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_GLOBAL(uint8_t, g_PageNameSentinel, 0x0082E65C);

static CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
               (void* self, void* edx, char* text, uint32_t length));
static CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
               (void* self, void* edx, void* source, uint32_t start, uint32_t count));
static CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0,
               (void* self, void* edx, void* name));
static CSF_FUNC(int, __fastcall, MenuManager_InitializeCachedInterface, 0x0050C030,
               (CMenuScreenFader* self, void* edx));
static CSF_FUNC(int, __cdecl, MenuManager_ContainsPageName, 0x0050C0A0,
               (char* name, void* names));
static CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
static CSF_FUNC(void*, __cdecl, MenuPage_LookupByName, 0x00541580,
               (void* string));
static CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310,
               (void* self, void* edx));
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110,
               (void* memory));
static CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330,
               (char value));
static CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
static CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20,
               (const char* name, char mode));
static CSF_FUNC(void*, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(char, __fastcall, MenuRuntime_SetPageByIndex, 0x006B15D0,
               (void* self, void* edx, void* name, int mode, int flags));
static CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41,
               (const char* left, const char* right));

static uint32_t MenuManager_GetStringLength(const char* text)
{
    const char* cursor = text;
    while (*cursor != '\0')
        ++cursor;
    return static_cast<uint32_t>(cursor - text);
}

static void MenuManager_InitializeTemporaryString(void* storage)
{
    CSF_AT(uint32_t, storage, 0x18) = 15;
    CSF_AT(uint32_t, storage, 0x14) = 0;
    CSF_AT(uint8_t, storage, 0x04) = 0;
}

static int __fastcall CMenuScreenFader_RequestScreen(
    CMenuScreenFader* self, void* /*edx*/, char* pageName)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    alignas(4) uint8_t temporaryString[0x1C];

    if (static_cast<uint8_t>(*pageName) != g_PageNameSentinel &&
        static_cast<uint8_t>(
            MenuManager_ContainsPageName(pageName, bytes + 0x38)) == 0)
    {
        const char previousOption = GetOptionsByte40();
        if (previousOption == 0)
            SetOptionsByteAt40(1);

        MenuManager_InitializeTemporaryString(temporaryString);
        const uint32_t length = MenuManager_GetStringLength(pageName);
        AssignStringRange_429B60(temporaryString, pageName, pageName, length);
        MenuPage_LookupByName(temporaryString);

        const char* temporaryName;
        if (CSF_AT(uint32_t, temporaryString, 0x18) < 16)
            temporaryName = reinterpret_cast<const char*>(temporaryString + 4);
        else
            temporaryName = reinterpret_cast<const char*>(
                CSF_AT(uint32_t, temporaryString, 0x04));

        const bool available =
            MenuManager_CheckPageAvailability(temporaryName, 0);
        SetOptionsByteAt40(previousOption);

        if (!available)
            return CMenuSaveGame_ResetStringStorage(temporaryString, nullptr);

        CMenuSaveGame_ResetStringStorage(temporaryString, nullptr);
    }

    const char* currentName;
    if (self->field_001c < 16)
        currentName = reinterpret_cast<const char*>(&self->field_0008);
    else
        currentName = reinterpret_cast<const char*>(self->field_0008);

    const int comparison = __stricmp(pageName, currentName);
    if (comparison == 0)
        return comparison;

    void* globalState = GetGlobalState_50D190();
    void* interfaceObject = CSF_AT(void*, globalState, 0x14);
    void* timingObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, interfaceObject, 0)[0x54 / 4])(
                interfaceObject, nullptr);
    void* timingData =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, timingObject, 0)[0x48 / 4])(
                timingObject, nullptr);

    const uint32_t durationBits = CSF_AT(uint32_t, timingData, 0x1C);
    std::memcpy(&self->field_0028, &durationBits, sizeof(durationBits));

    const uint32_t length = MenuManager_GetStringLength(pageName);
    AssignStringRange_429B60(bytes + 4, pageName + 1, pageName, length);

    if (static_cast<uint8_t>(
            MenuManager_ContainsPageName(pageName, bytes + 0x38)) == 0)
    {
        MenuManager_AppendPageName(bytes + 0x38, nullptr, bytes + 4);
    }

    if (self->m_state == 3)
    {
        const double reversedProgress =
            static_cast<double>(self->field_0028) -
            static_cast<double>(self->field_0030);
        self->m_state = 1;
        self->m_bIdle = 0;
        self->field_0030 = static_cast<float>(reversedProgress);
        return MenuManager_InitializeCachedInterface(self, nullptr);
    }

    if (self->m_bIdle != 0)
    {
        if (self->field_0048 != 0)
        {
            Mem_Free(
                reinterpret_cast<void*>(self->field_0048));
            self->field_0048 = 0;
        }

        MenuManager_InitializeTemporaryString(temporaryString);
        AssignStringSubstring(
            temporaryString, nullptr, bytes + 4, 0, UINT32_MAX);
        void* processedName = MenuPage_LookupByName(temporaryString);
        void* controller = GetGlobalInt();
        MenuRuntime_SetPageByIndex(controller, nullptr, processedName, 2, 0);
        self->m_state = 3;
        CMenuSaveGame_ResetStringStorage(temporaryString, nullptr);
    }
    else
    {
        self->m_state = 1;
    }

    self->field_0030 = 0.0f;
    self->m_bIdle = 0;
    return MenuManager_InitializeCachedInterface(self, nullptr);
}

HOOK(0x0050C660, CMenuScreenFader_RequestScreen, "auto");
