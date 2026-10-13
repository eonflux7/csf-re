#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_ComponentPools, 0x008BAF48);

CSF_FUNC(int, __fastcall, CoreParser_ReadIndexedEntry, 0x00692B50, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static char __fastcall CoreParser_CopyAttributeText(
    uint8_t* self, void* /*edx*/, char* buffer, int capacity)
{
    for (;;) {
        if (CSF_AT(uint8_t, self, 0xB) == 0xFF) {
            void* reference = CSF_AT(void*, self, 0);
            if (CSF_AT(uint32_t, reference, 0) != 0) {
                void* text = CSF_AT(void*, reference, 0x4);
                uint32_t count = static_cast<uint32_t>(capacity) - 1;
                uint32_t length = CSF_AT(uint32_t, text, 0);
                if (length <= count)
                    count = length;

                std::memcpy(buffer, CSF_AT(const void*, text, 0x4), count);
                buffer[count] = '\0';
                return static_cast<char>(count);
            }
            self = CSF_AT(uint8_t*, reference, 0x18);
            continue;
        }

        if (CSF_AT(uint8_t, self, 0xA) != 0)
            break;

        void* pool = CSF_AT(
            void*, &g_ComponentPools,
            4 * CSF_AT(uint8_t, self, 0xB));
        uint32_t displacement = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(self) -
            reinterpret_cast<uintptr_t>(CSF_AT(uint8_t*, pool, 0)));
        uint32_t nextIndex = displacement / 0xC + 1;
        uint8_t poolFlag = CSF_AT(uint8_t, pool, 0x14);
        self = reinterpret_cast<uint8_t*>(
            reinterpret_cast<uintptr_t>(CSF_AT(uint8_t*, pool, 0)) +
            12 * nextIndex);
        if (poolFlag == 0)
            CSF_AT(uint8_t, self, 0xB) = CSF_AT(uint8_t, pool, 0x15);
    }

    uint8_t type = CSF_AT(uint8_t, self, 0xA);
    switch (type) {
    case 3:
        return static_cast<char>(_sprintf(
            buffer, CSF_STR(0x0083090C, "%d"),
            CSF_AT(int, self, 0x4)));

    case 4:
        return static_cast<char>(_sprintf(
            buffer, CSF_STR(0x00840C3C, "%f"),
            static_cast<double>(CSF_AT(float, self, 0x4))));

    case 5: {
        void* pool = CSF_AT(
            void*, &g_ComponentPools,
            4 * CSF_AT(uint8_t, self, 0xB));
        int entry = CoreParser_ReadIndexedEntry(
            pool, nullptr, CSF_AT(int, self, 0x4));
        if (entry == 0)
            return 0;

        const char* source = reinterpret_cast<const char*>(
            static_cast<uintptr_t>(static_cast<uint32_t>(entry)));
        char* destination = buffer;
        char character;
        do {
            character = *source;
            *destination++ = character;
            ++source;
        } while (character != '\0');
        return static_cast<char>(reinterpret_cast<uintptr_t>(source));
    }

    default:
        return static_cast<char>(type);
    }
}

HOOK(0x692D40, CoreParser_CopyAttributeText, "auto");
