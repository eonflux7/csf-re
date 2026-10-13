#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_44ACE0, 0x0044ACE0, ());
CSF_FUNC(int, __fastcall, Global_CheckEntry, 0x0044AF70, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

static int __fastcall C_00830e5c_SelectRadarMarker(
    C_00830e5c* self, void* /*edx*/, uint32_t* entry)
{
    switch (entry[30])
    {
    case 0:
        if ((entry[92] & 0x2000) != 0)
        {
            void* global = reinterpret_cast<void*>(GetGlobalValue_44ACE0());
            if (static_cast<uint8_t>(
                    Global_CheckEntry(global, nullptr, reinterpret_cast<int>(entry + 7))) != 0)
            {
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x28);
            }
            if (entry[21] != 0)
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x00);
            return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x14);
        }

        if (static_cast<uint8_t>(
                GameConfig_IsResolvedEntryActive(static_cast<int>(entry[20]))) == 0)
            return 0;
        {
            void* global = reinterpret_cast<void*>(GetGlobalValue_44ACE0());
            if (static_cast<uint8_t>(
                    Global_CheckEntry(global, nullptr, reinterpret_cast<int>(entry + 7))) != 0)
            {
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x34);
            }
            return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x18);
        }

    case 1:
        if ((entry[92] & 0x2000) != 0)
        {
            void* global = reinterpret_cast<void*>(GetGlobalValue_44ACE0());
            if (static_cast<uint8_t>(
                    Global_CheckEntry(global, nullptr, reinterpret_cast<int>(entry + 7))) != 0)
            {
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x2C);
            }

            switch (entry[118])
            {
            case 0:
            {
                void* object = reinterpret_cast<void*>(entry[13]);
                bool alternate = false;
                if (object != nullptr)
                {
                    alternate = ((bool(__fastcall*)(void*, void*))
                        CSF_AT(void**, object, 0)[0xB0 / 4])(object, nullptr);
                }
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c),
                              4 + 4 * (alternate != 0));
            }
            case 1:
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x08);
            case 2:
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x0C);
            case 3:
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x10);
            default:
                return 0;
            }
        }

        if (static_cast<uint8_t>(
                GameConfig_IsResolvedEntryActive(static_cast<int>(entry[20]))) == 0)
            return 0;
        {
            void* global = reinterpret_cast<void*>(GetGlobalValue_44ACE0());
            if (static_cast<uint8_t>(
                    Global_CheckEntry(global, nullptr, reinterpret_cast<int>(entry + 7))) != 0)
            {
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x38);
            }
            return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x1C);
        }

    case 2:
        if ((entry[92] & 0x2000) != 0)
        {
            void* global = reinterpret_cast<void*>(GetGlobalValue_44ACE0());
            if (static_cast<uint8_t>(
                    Global_CheckEntry(global, nullptr, reinterpret_cast<int>(entry + 7))) != 0)
            {
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x30);
            }
            return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x20);
        }

        if (static_cast<uint8_t>(
                GameConfig_IsResolvedEntryActive(static_cast<int>(entry[20]))) == 0)
            return 0;
        {
            void* global = reinterpret_cast<void*>(GetGlobalValue_44ACE0());
            if (static_cast<uint8_t>(
                    Global_CheckEntry(global, nullptr, reinterpret_cast<int>(entry + 7))) != 0)
            {
                return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x3C);
            }
            return CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x24);
        }

    default:
        return 0;
    }
}

HOOK(0x0043D070, C_00830e5c_SelectRadarMarker, "auto");
