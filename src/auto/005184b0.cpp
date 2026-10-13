#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Compares two NUL-terminated strings byte by byte, the same way the original inlines strcmp.
static bool NetProfile_NameMatches(const char* candidate, const char* stored) {
    const uint8_t* a = reinterpret_cast<const uint8_t*>(candidate);
    const uint8_t* b = reinterpret_cast<const uint8_t*>(stored);
    while (true) {
        if (*a != *b) {
            return false;
        }
        if (*a == 0) {
            return true;
        }
        ++a;
        ++b;
    }
}

// Scans the 10 record slots of the profile object (virtual slot 12, byte offset 0x30)
// and returns true if one has a matching name at +8 and a matching lead byte at +0.
static bool __fastcall NetProfile_CheckAccountName(void* self, void* /*edx*/,
                                                   const char* accountName,
                                                   char expectedLeadByte) {
    using GetRecordFn = void* (__fastcall*)(void* self, void* edx, int slotIndex);

    for (int slotIndex = 0; slotIndex < 10; ++slotIndex) {
        GetRecordFn getRecord = reinterpret_cast<GetRecordFn>(CSF_AT(void**, self, 0)[0x30 / 4]);
        uint8_t* record = static_cast<uint8_t*>(getRecord(self, nullptr, slotIndex));
        if (record == nullptr) {
            continue;
        }

        const char* storedName = reinterpret_cast<const char*>(record + 8);
        if (!NetProfile_NameMatches(accountName, storedName)) {
            continue;
        }

        if (*record != static_cast<uint8_t>(expectedLeadByte)) {
            continue;
        }

        return true;
    }
    return false;
}

HOOK(0x005184B0, NetProfile_CheckAccountName, "auto");
