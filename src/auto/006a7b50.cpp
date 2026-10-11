#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_006a7640_SetGatheringStatusCode(
    S_006a7640* self, void* /*edx*/, int operation, int* status)
{
    int result = operation;

    switch (operation)
    {
    case 0:
        self->field_02b0 = 1229731634;
        result = static_cast<int>(static_cast<uint32_t>(*status) + 2147287039);
        switch (*status)
        {
        case -2147287039: self->field_02b0 = 1229731379; break;
        case -2147287038: self->field_02b0 = 1229731380; break;
        case -2147286940: self->field_02b0 = 1229730614; break;
        case -2147286939: self->field_02b0 = 1229730617; break;
        case -2147286937: self->field_02b0 = 1229730871; break;
        case -2147286936: self->field_02b0 = 1229730872; break;
        default: return result;
        }
        break;

    case 1:
        self->field_02b0 = 1229731633;
        result = static_cast<int>(static_cast<uint32_t>(*status) + 2147287039);
        switch (*status)
        {
        case -2147287039: self->field_02b0 = 1229731129; break;
        case -2147287038: self->field_02b0 = 1229731376; break;
        case -2147286940: self->field_02b0 = 1229731377; break;
        case -2147286939: self->field_02b0 = 1229731378; break;
        case -2147286938: self->field_02b0 = 1229731128; break;
        case -2147286937: self->field_02b0 = 1229731125; break;
        case -2147286936: self->field_02b0 = 1229731126; break;
        case -2147286935: self->field_02b0 = 1229731127; break;
        default: return result;
        }
        break;

    case 2:
        self->field_02b0 = 1229731635;
        result = static_cast<int>(static_cast<uint32_t>(*status) + 2147287039);
        switch (*status)
        {
        case -2147287039: self->field_02b0 = 1229731381; break;
        case -2147287038: self->field_02b0 = 1229731382; break;
        case -2147286933: self->field_02b0 = 1229731383; break;
        default: return result;
        }
        break;

    case 3:
        self->field_02b0 = 1229731632;
        result = *status;
        if (*status > -2147155967)
        {
            result = static_cast<int>(static_cast<uint32_t>(result) + 2147155966);
            if (!result)
                self->field_02b0 = 1229732149;
        }
        else if (result != -2147155967)
        {
            result = static_cast<int>(static_cast<uint32_t>(result) + 2147287039);
            switch (result)
            {
            case 0:   self->field_02b0 = 1229731121; break;
            case 99:  self->field_02b0 = 1229731122; break;
            case 100: self->field_02b0 = 1229731123; break;
            case 102: self->field_02b0 = 1229730873; break;
            case 103: self->field_02b0 = 1229731120; break;
            case 104: self->field_02b0 = 1229731124; break;
            default: return result;
            }
        }
        break;

    case 4:
        self->field_02b0 = 1229731384;
        result = static_cast<int>(static_cast<uint32_t>(*status) + 2147287039);
        if (*status == -2147287038)
            self->field_02b0 = 1229731385;
        break;

    default:
        return result;
    }

    return result;
}

HOOK(0x006A7B50, S_006a7640_SetGatheringStatusCode, "auto");
