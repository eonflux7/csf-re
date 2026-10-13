#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint8_t*, __fastcall, CoreZlibLog_ReadWideAttributeText, 0x00692E40, (void* self, void* edx, int64_t a2));
CSF_FUNC(uint8_t*, __cdecl, CoreZlibLog_ConvertAttributeTextToWide, 0x006955E0, (uint16_t* a1, uint16_t* a2, int a3));

// Reads an attribute text as wide characters. With no inner record it delegates to
// CoreZlibLog_ReadWideAttributeText; otherwise it copies the narrow text (capped at 1023 bytes)
// into a local buffer and converts it to wide form.
static uint8_t* __fastcall CoreZlibLog_ReadFallbackWideAttributeText(void** self, void* /*edx*/, int64_t a2)
{
    if (*self == nullptr)
    {
        void* innerObject = CSF_AT(void*, self, 0x18);
        return CoreZlibLog_ReadWideAttributeText(innerObject, nullptr, a2);
    }

    uint8_t* textRecord = CSF_AT(uint8_t*, self, 4);
    uint32_t textLength = CSF_AT(uint32_t, textRecord, 0);
    if (textLength > 0x3FFu)
        textLength = 0x3FFu;

    uint8_t* textData = CSF_AT(uint8_t*, textRecord, 4);
    uint16_t wideText[512];
    memcpy(wideText, textData, textLength);
    reinterpret_cast<uint8_t*>(wideText)[textLength] = 0;

    uint16_t* outBuffer = reinterpret_cast<uint16_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    int outHigh = static_cast<int>(a2 >> 32);
    return CoreZlibLog_ConvertAttributeTextToWide(wideText, outBuffer, outHigh);
}

HOOK(0x00693460, CoreZlibLog_ReadFallbackWideAttributeText, "auto");
