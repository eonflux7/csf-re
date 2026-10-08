#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static int __fastcall MenuMessageBox_ResetTextPlaceholders(
    void* self, void* /*edx*/)
{
    void* object = CSF_AT(void*, self, 0x5C);
    if (object)
    {
        void* text = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x110 / 4])(object, nullptr);

        if (text)
        {
            ((int (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, text, 0)[0x3C / 4])(
                    text, nullptr, CSF_STR(0x00841244, "@@8/@@8/@@8/@@8/@@8/@@8/@@8/@@8/@@8/@@8/"));
        }
    }

    auto* entry = reinterpret_cast<std::uint8_t*>(self) + 0x60;
    int result;
    for (int remaining = 3; remaining != 0; --remaining, entry += 4)
    {
        object = CSF_AT(void*, entry, 0);
        result = reinterpret_cast<int>(object);
        if (object)
        {
            void* text = ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x110 / 4])(object, nullptr);

            result = reinterpret_cast<int>(text);
            if (text)
            {
                result = ((int (__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, text, 0)[0x3C / 4])(
                        text, nullptr, CSF_STR(0x0084123C, "@@9/"));
            }
        }
    }
    return result;
}

HOOK(0x00528A80, MenuMessageBox_ResetTextPlaceholders, "auto");
