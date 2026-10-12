#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Searches the table of session-map names held by C_0082d410 for the name in
// a2 (a legacy std::string-like object) and returns the matching entry, or nullptr.
static const char* __fastcall C_0082d410_FindSessionMapDefinition(C_0082d410* self, void* /*edx*/, int a2)
{
    const char* result = nullptr;
    int entryCount = CSF_AT(int, self, 0x10);
    if (entryCount <= 0)
        return result;

    uintptr_t keyAddress = static_cast<uint32_t>(a2);
    uintptr_t entryArrayAddress = self->field_0004;

    for (int index = 0; index < entryCount; ++index)
    {
        // Short strings (capacity < 16) keep their characters inline at +4;
        // longer ones keep a pointer to the buffer at +4.
        uint32_t keyCapacity = CSF_AT(uint32_t, reinterpret_cast<void*>(keyAddress), 0x18);
        const char* keyText;
        if (keyCapacity < 0x10u)
            keyText = reinterpret_cast<const char*>(keyAddress + 4);
        else
            keyText = CSF_AT(const char*, reinterpret_cast<void*>(keyAddress), 4);

        const char** entries = reinterpret_cast<const char**>(entryArrayAddress);
        if (strcmp(entries[index], keyText) == 0)
            return entries[index];
    }
    return nullptr;
}

HOOK(0x0054E630, C_0082d410_FindSessionMapDefinition, "auto");
