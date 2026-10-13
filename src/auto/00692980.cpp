#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static std::uint8_t* __fastcall CoreParser_FindChildByName(
    void* self, void* /*edx*/, char* name)
{
    if (CSF_AT(std::uint32_t, self, 0x00) != 2)
        return nullptr;

    const std::uint32_t count =
        CSF_AT(std::uint32_t, self, 0x40) +
        CSF_AT(std::uint32_t, self, 0x10);
    if (count == 0)
        return nullptr;

    alignas(8) std::uint8_t nameBytes[0x114];
    char* candidateName = reinterpret_cast<char*>(nameBytes);
    auto* inlineSlot = reinterpret_cast<std::uint8_t*>(self) + 0x18;

    for (std::uint32_t index = 0; index < count; ++index, inlineSlot += 4)
    {
        std::uint8_t* child;
        if (index >= 0xA)
            child = CSF_AT(std::uint8_t**, self, 0x08)[index - 0xA];
        else
            child = CSF_AT(std::uint8_t*, inlineSlot, 0x00);

        void* nameDescriptor = nullptr;
        if (CSF_AT(std::uint8_t, child, 0x0B) == 0xFF)
        {
            void* storage = CSF_AT(void*, child, 0x00);
            nameDescriptor = CSF_AT(void*, storage, 0x04);
        }

        std::uint32_t length = CSF_AT(std::uint32_t, nameDescriptor, 0x00);
        if (length > 0xFFu)
            length = 255;

        const void* source = CSF_AT(const void*, nameDescriptor, 0x04);
        std::memcpy(candidateName, source, length);
        candidateName[length] = 0;

        if (name && __stricmp(name, candidateName) == 0)
            return child;
    }

    return nullptr;
}

HOOK(0x00692980, CoreParser_FindChildByName, "auto");
