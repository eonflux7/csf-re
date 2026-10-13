#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, DevConsole_GrowEntryStorage, 0x004FAB30, (void* self, void* , int additionalCapacity));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, StringFormat_GetWideTextLength, 0x00695560, (uint16_t* text));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(wchar_t*, __cdecl, _wcscpy, 0x007B004C, (wchar_t* Destination, wchar_t* Source));
CSF_FUNC(wchar_t*, __cdecl, _wcsncpy, 0x007B0068, (wchar_t* Destination, wchar_t* Source, uint32_t Count));

static char __fastcall S_004faea0_OpenMenu(
    S_004faea0* self, void* /*edx*/, int entryType, wchar_t* source)
{
    alignas(8) uint8_t entryBytes[0x138] = {};
    wchar_t* entryText = reinterpret_cast<wchar_t*>(entryBytes + 4);
    wchar_t* remainingText = source;
    char finished = 0;

    do
    {
        CSF_AT(int, entryBytes, 0) = entryType;

        void* object = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* metrics =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

        int textLength = StringFormat_GetWideTextLength(
            reinterpret_cast<uint16_t*>(remainingText));
        int lineLength = CSF_AT(int32_t, metrics, 0) / 11;

        if (textLength <= lineLength)
        {
            _wcscpy(entryText, remainingText);
            finished = 1;
        }
        else
        {
            object = LookupObjectById(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            metrics = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

            _wcsncpy(entryText, source,
                static_cast<uint32_t>(CSF_AT(int32_t, metrics, 0) / 11 - 1));

            object = LookupObjectById(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            metrics = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
            entryText[CSF_AT(int32_t, metrics, 0) / 11] = 0;

            object = LookupObjectById(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr);
            metrics = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
            remainingText += CSF_AT(int32_t, metrics, 0) / 11 - 1;
        }

        uint8_t* storage = reinterpret_cast<uint8_t*>(self) + 0x16C;
        if (self->field_0174 >= CSF_AT(uint32_t, self, 0x170))
        {
            DevConsole_GrowEntryStorage(
                storage, nullptr, CSF_AT(int, self, 0x178));
        }

        uint32_t entryIndex = self->field_0174;
        uint8_t* entries = CSF_AT(uint8_t*, self, 0x16C);
        uint8_t* destination = entries + 0x130 * entryIndex;
        self->field_0174 = entryIndex + 1;
        std::memcpy(destination, entryBytes, 0x130);

        object = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        uint32_t entryCount = self->field_0174;
        metrics = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

        if (entryCount > (CSF_AT(uint32_t, metrics, 4) >> 1) / 0x14u - 2)
            ++CSF_AT(uint32_t, self, 0x160);
    }
    while (!finished);

    return finished;
}

HOOK(0x004FAEA0, S_004faea0_OpenMenu, "auto");
