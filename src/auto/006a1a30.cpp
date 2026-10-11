#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, sub_6A4F10, 0x006A4F10, (int* a1, void** a2, int a3));
CSF_FUNC(int, __cdecl, ZlibStream_ResetState, 0x006A4DF0,
         (void* a1, void* a2, uint32_t* a3));

static int __cdecl CoreZlibLog_Inflate(uint8_t** stream, int flush)
{
    if (!stream || !CSF_AT(void*, stream, 0x1C) || !*stream)
        return -2;

    int result = -5;
    const int progressResult = flush != 4 ? 0 : -5;

    for (;;)
    {
        switch (CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00))
        {
        case 0:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) = **stream;
            result = progressResult;
            ++*stream;

            if ((CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) & 0xF) != 8)
            {
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 13;
                CSF_AT(const char*, stream, 0x18) =
                    CSF_STR(0x0085EDA8, "unknown compression method");
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) = 5;
                break;
            }
            if ((CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) >> 4) + 8 >
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x10))
            {
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 13;
                CSF_AT(const char*, stream, 0x18) =
                    CSF_STR(0x0085ED94, "invalid window size");
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) = 5;
                break;
            }
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 1;
            [[fallthrough]];

        case 1:
        {
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            void* state = CSF_AT(void*, stream, 0x1C);
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            uint32_t flags = *(*stream)++;
            result = progressResult;
            if ((flags + (CSF_AT(uint32_t, state, 0x04) << 8)) % 0x1Fu)
            {
                CSF_AT(uint32_t, state, 0x00) = 13;
                CSF_AT(const char*, stream, 0x18) =
                    CSF_STR(0x0085ED7C, "incorrect header check");
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) = 5;
                break;
            }
            if ((flags & 0x20) == 0)
            {
                CSF_AT(uint32_t, state, 0x00) = 7;
                break;
            }
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 2;
        }
            [[fallthrough]];

        case 2:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            ++CSF_AT(uint32_t, stream, 0x08);
            --CSF_AT(uint32_t, stream, 0x04);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) =
                static_cast<uint32_t>(**stream) << 24;
            ++*stream;
            result = progressResult;
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 3;
            [[fallthrough]];

        case 3:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) +=
                static_cast<uint32_t>(**stream) << 16;
            ++*stream;
            result = progressResult;
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 4;
            [[fallthrough]];

        case 4:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) +=
                static_cast<uint32_t>(**stream) << 8;
            ++*stream;
            result = progressResult;
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 5;
            [[fallthrough]];

        case 5:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) += **stream;
            ++*stream;
            CSF_AT(uint32_t, stream, 0x30) =
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 6;
            return 2;

        case 6:
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 13;
            CSF_AT(const char*, stream, 0x18) =
                CSF_STR(0x0085ED54, "need dictionary");
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) = 0;
            return -2;

        case 7:
            result = sub_6A4F10(
                CSF_AT(int*, CSF_AT(void*, stream, 0x1C), 0x14),
                reinterpret_cast<void**>(stream), result);
            if (result == -3)
            {
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 13;
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) = 0;
                break;
            }
            if (result == 0)
                result = progressResult;
            if (result != 1)
                return result;

            result = progressResult;
            ZlibStream_ResetState(
                CSF_AT(void*, CSF_AT(void*, stream, 0x1C), 0x14),
                stream,
                &CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04));
            if (CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x0C))
            {
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 12;
                break;
            }
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 8;
            [[fallthrough]];

        case 8:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) =
                static_cast<uint32_t>(**stream) << 24;
            ++*stream;
            result = progressResult;
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 9;
            [[fallthrough]];

        case 9:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) +=
                static_cast<uint32_t>(**stream) << 16;
            ++*stream;
            result = progressResult;
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 10;
            [[fallthrough]];

        case 10:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) +=
                static_cast<uint32_t>(**stream) << 8;
            ++*stream;
            result = progressResult;
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 11;
            [[fallthrough]];

        case 11:
            if (!CSF_AT(uint32_t, stream, 0x04))
                return result;
            --CSF_AT(uint32_t, stream, 0x04);
            ++CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08) += **stream;
            ++*stream;
            result = progressResult;
            if (CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) ==
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x08))
            {
                CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 12;
                return 1;
            }
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x00) = 13;
            CSF_AT(const char*, stream, 0x18) =
                CSF_STR(0x0085ED64, "incorrect data check");
            CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x04) = 5;
            break;

        case 12:
            return 1;

        case 13:
            return -3;

        default:
            return -2;
        }
    }
}

HOOK(0x006A1A30, CoreZlibLog_Inflate, "auto");
