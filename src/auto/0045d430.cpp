#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Compares the 0x10/0x14 key pairs of two operand records (A and B) and writes the
// result as a 0/1 flag into the owner record's flag byte at +4 -> +1.
// Flag mode 0: store 1 when keys match, else 0.
// Flag mode 1: store 0 when keys match, else 1.
// Any other mode: no store; returns the flag record pointer's value with its low byte replaced by the mode.
static int __stdcall EvaluateFlaggedPairMatch(int ownerRecord, int /*unused*/, uint32_t* operands)
{
    uint32_t objX = operands[0];
    uint32_t objY = operands[1];
    uint32_t objZ = operands[2];

    uint32_t flagRecord = *reinterpret_cast<uint32_t*>(objY + 4);
    uint32_t keyRecordA = *reinterpret_cast<uint32_t*>(objX + 4);
    uint32_t keyRecordB = *reinterpret_cast<uint32_t*>(objZ + 4);
    uint8_t flagMode = *reinterpret_cast<uint8_t*>(flagRecord + 1);

    uint32_t aMajor = *reinterpret_cast<uint32_t*>(keyRecordA + 0x10);
    uint32_t aMinor = *reinterpret_cast<uint32_t*>(keyRecordA + 0x14);
    uint32_t bMajor = *reinterpret_cast<uint32_t*>(keyRecordB + 0x10);
    uint32_t bMinor = *reinterpret_cast<uint32_t*>(keyRecordB + 0x14);
    bool keysMatch = (aMajor == bMajor) && (aMinor == bMinor);

    if (flagMode > 1)
    {
        // Unrecognised mode: returns the pointer value with the mode in its low byte, nothing stored.
        return static_cast<int>((flagRecord & 0xFFFFFF00u) | flagMode);
    }

    uint8_t newFlag = (flagMode == 0) ? (keysMatch ? 1 : 0) : (keysMatch ? 0 : 1);

    uint32_t ownerAddr = static_cast<uint32_t>(ownerRecord);
    uint32_t targetRecord = *reinterpret_cast<uint32_t*>(ownerAddr + 4);
    *reinterpret_cast<uint8_t*>(targetRecord + 1) = newFlag;
    return static_cast<int>(newFlag);
}

HOOK(0x45D430, EvaluateFlaggedPairMatch, "auto");
