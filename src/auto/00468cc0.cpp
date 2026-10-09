#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes objectId from the id list of the table slot selected by a3 (a3 & 0x8000000 maps to slot 1).
// Each table record is 16 bytes: +0 int* id array, +8 id count.
static int __fastcall S_0045d610_UnbindObjectFromId(S_0045d610* self, void* /*edx*/, int objectId, int slotKey)
{
    int result = objectId;
    if (objectId != 0)
    {
        result = slotKey;
        if (slotKey != -1)
        {
            if ((slotKey & 0x8000000) != 0)
                result = 1;

            int recordOffset = 16 * result;
            uint32_t tableBase = self->field_0078;
            int idCount = CSF_AT(int, reinterpret_cast<uint8_t*>(tableBase), recordOffset + 8);
            result = static_cast<int>(tableBase + recordOffset);

            for (int i = idCount - 1; i >= 0; --i)
            {
                uint8_t* record = reinterpret_cast<uint8_t*>(self->field_0078) + recordOffset;
                int* ids = CSF_AT(int*, record, 0);
                result = objectId;
                if (ids[i] == objectId)
                {
                    result = CSF_AT(int, record, 8) - 1;
                    CSF_AT(int, record, 8) = result;
                    if (i != result)
                    {
                        result = reinterpret_cast<int>(
                            _memcpy(ids + i, ids + i + 1, static_cast<std::size_t>(4 * (result - i))));
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x468CC0, S_0045d610_UnbindObjectFromId, "auto");
