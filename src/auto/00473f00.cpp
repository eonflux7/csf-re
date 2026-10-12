#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RecordFlagAndDispatchOnce_405CA0, 0x00405CA0, (void* self, int a2, int a3, char a4, char a5));
CSF_FUNC(int, __cdecl, RecordFlagAndDispatchOnce_405CE0, 0x00405CE0, (void* self, int a2, int a3, char a4, char a5));
CSF_FUNC(int, __cdecl, SetFlagAndDispatchOnce, 0x00407450, (void* self, int a2, int a3, char a4, char a5));
CSF_FUNC(int, __cdecl, StoreFlagAndLatchDispatchResult, 0x00408A90, (void* self, int a2, int a3, char a4, char a5));
CSF_FUNC(int, __cdecl, RecordStateAndDispatchOnce, 0x00410700, (void* self, int a2, int a3, char a4, char a5));
CSF_FUNC(S_00691860*, __cdecl, SerializeStringWithFlags, 0x00413540, (S_00691860* a2, int a1, uint32_t a3, char a4, char a5));
CSF_FUNC(int, __cdecl, Campaign_SetBriefingRecordAndSerialize, 0x00440BF0, (int a1, C_0082d410* a2, int a3, char a4, char a5));
CSF_FUNC(int, __cdecl, RecordSerialization_SerializeCompactPayload, 0x004722C0, (int a1, int a2, int a3, char a4, char a5));
CSF_FUNC(int, __cdecl, RecordSerialization_SerializeExtendedPayload, 0x00473E80, (int a1, int a2, int a3, char a4, char a5, int a6, int a7, int a8, int a9, int a10));

static char* __fastcall C_008313f8_SerializeRecord(
    C_008313f8* self, void* /*edx*/, S_00691860* serializer)
{
    char* result = reinterpret_cast<char*>(self->field_0004);

    switch (*result)
    {
    case 1:
        RecordStateAndDispatchOnce(
            serializer, reinterpret_cast<int>(result + 8), 0, 0, 0);
        result = reinterpret_cast<char*>(
            RecordSerialization_SerializeExtendedPayload(
                reinterpret_cast<int>(serializer),
                static_cast<int>(self->field_0004 + 12),
                0, 0, 0, 0, 0, 0, 0, 0));
        break;

    case 4:
        result = reinterpret_cast<char*>(SerializeStringWithFlags(
            serializer, reinterpret_cast<int>(result + 4), 0, 0, 0));
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x10) =
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x8);
        break;

    case 5:
        StoreFlagAndLatchDispatchResult(
            serializer, reinterpret_cast<int>(result + 4), 0, 0, 0);
        RecordStateAndDispatchOnce(
            serializer, static_cast<int>(self->field_0004 + 16), 0, 0, 0);
        result = reinterpret_cast<char*>(RecordStateAndDispatchOnce(
            serializer, static_cast<int>(self->field_0004 + 20), 0, 0, 0));
        break;

    case 6:
    case 7:
    case 10:
    case 13:
    case 14:
    case 25:
    case 26:
    case 27:
    case 29:
    case 30:
    case 32:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
        result = reinterpret_cast<char*>(RecordStateAndDispatchOnce(
            serializer, reinterpret_cast<int>(result + 4), 0, 0, 0));
        break;

    case 11:
        RecordStateAndDispatchOnce(
            serializer, reinterpret_cast<int>(result + 32), 0, 0, 0);
        result = reinterpret_cast<char*>(Campaign_SetBriefingRecordAndSerialize(
            reinterpret_cast<int>(serializer),
            reinterpret_cast<C_0082d410*>(self->field_0004 + 4), 0, 0, 0));
        break;

    case 20:
        result = reinterpret_cast<char*>(StoreFlagAndLatchDispatchResult(
            serializer, reinterpret_cast<int>(result + 4), 0, 0, 0));
        break;

    case 21:
        result = reinterpret_cast<char*>(RecordFlagAndDispatchOnce_405CE0(
            serializer, reinterpret_cast<int>(result + 1), 0, 0, 0));
        break;

    case 22:
        result = reinterpret_cast<char*>(RecordFlagAndDispatchOnce_405CA0(
            serializer, reinterpret_cast<int>(result + 4), 0, 0, 0));
        break;

    case 23:
        result = reinterpret_cast<char*>(Campaign_SetBriefingRecordAndSerialize(
            reinterpret_cast<int>(serializer),
            reinterpret_cast<C_0082d410*>(result + 4), 0, 0, 0));
        break;

    case 24:
    case 36:
    case 38:
        result = reinterpret_cast<char*>(SetFlagAndDispatchOnce(
            serializer, reinterpret_cast<int>(result + 4), 0, 0, 0));
        break;

    case 31:
        RecordStateAndDispatchOnce(
            serializer, reinterpret_cast<int>(result + 4), 0, 0, 0);
        [[fallthrough]];
    case 12:
        result = reinterpret_cast<char*>(RecordStateAndDispatchOnce(
            serializer, static_cast<int>(self->field_0004 + 4), 0, 0, 0));
        break;

    case 33:
    case 34:
    case 35:
        result = reinterpret_cast<char*>(
            RecordSerialization_SerializeCompactPayload(
                reinterpret_cast<int>(serializer),
                reinterpret_cast<int>(result + 1), 0, 0, 0));
        break;

    default:
        break;
    }

    return result;
}

HOOK(0x00473F00, C_008313f8_SerializeRecord, "auto");
