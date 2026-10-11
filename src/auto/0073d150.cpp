#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(int, __cdecl, _sscanf, 0x007B213C, (void* Buffer, void* Format, ...));
CSF_FUNC(void, __fastcall, NetSession_ReadPropertyString, 0x00680360,
         (void* self, void* edx, const char* name, char* destination, unsigned int capacity));

static int __fastcall NetSession_CreateSessionInfo(void* self, void* /*edx*/, int sessionIndex)
{
    const int state = CSF_AT(int, self, 0x98);
    if (state == 6 || state == 0)
        return 0;

    void* sentinel = CSF_AT(void*, self, 0xA0);
    void* node = CSF_AT(void*, sentinel, 0);
    if (node == sentinel)
        return 0;

    alignas(8) uint8_t crcBytes[0x80];
    alignas(8) uint8_t numberBytes[0x14];
    char* crcText = reinterpret_cast<char*>(crcBytes);
    char* numberText = reinterpret_cast<char*>(numberBytes);
    void* decimalFormat = const_cast<char*>(CSF_STR(0x0083090C, "%d"));

    uint8_t* info = nullptr;
    uint32_t index = 0;
    do
    {
        if (index == static_cast<uint32_t>(sessionIndex))
        {
            info = static_cast<uint8_t*>(Mem_Alloc(400));
            CSF_AT(int, info, 0) = sessionIndex;

            void* session = static_cast<uint8_t*>(node) + 8;
            const char* sessionName =
                reinterpret_cast<const char*>(GetCurrentItemCount(session, nullptr));
            CopyTrucoStringToWide(sessionName,
                                  reinterpret_cast<uint16_t*>(info + 4), 127);

            NetSession_ReadPropertyString(
                session, nullptr, CSF_STR(0x0086762C, "NumJugs"), numberText, 0xA);
            _sscanf(numberText, decimalFormat, &CSF_AT(int, info, 0x184));

            NetSession_ReadPropertyString(
                session, nullptr, CSF_STR(0x00867624, "MaxJugs"), numberText, 0xA);
            _sscanf(numberText, decimalFormat, &CSF_AT(int, info, 0x188));

            NetSession_ReadPropertyString(
                session, nullptr, CSF_STR(0x00867634, "NombreMapa"),
                reinterpret_cast<char*>(info + 0x104), 0x80);

            NetSession_ReadPropertyString(
                session, nullptr, CSF_STR(0x00867654, "CRC"), crcText, 0x80);
            _sscanf(crcText, decimalFormat, &CSF_AT(int, info, 0x18C));
        }

        node = CSF_AT(void*, node, 0);
        sentinel = CSF_AT(void*, self, 0xA0);
        ++index;
    } while (node != sentinel);

    return static_cast<int>(reinterpret_cast<std::intptr_t>(info));
}

HOOK(0x0073D150, NetSession_CreateSessionInfo, "auto");
