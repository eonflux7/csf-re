#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MotionCompletionEpsilon, 0x008614E0);

static int __fastcall C_00861508_UpdateMotion(
    C_00861508* self, void* /*edx*/, int deltaTimeBits)
{
    // The caller tests AL but forwards the entire EAX result.
    const int flagResult =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            self->vtbl[0x34 / 4])(self, nullptr);
    if (static_cast<uint8_t>(flagResult))
        return flagResult;

    C_0085ec18_NullSub(deltaTimeBits);

    float deltaTime;
    std::memcpy(&deltaTime, &deltaTimeBits, sizeof(deltaTime));

    auto* definition = reinterpret_cast<uint8_t*>(self->field_0018);
    const float elapsed =
        deltaTime * CSF_AT(float, definition, 0x5C) + self->field_0080;
    self->field_0080 = elapsed;
    const float remaining = self->field_0084 - elapsed;

    if (remaining <= g_MotionCompletionEpsilon)
    {
        // This path likewise forwards EAX, despite the void slot wrapper.
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            self->vtbl[0x2C / 4])(self, nullptr, 1);
    }

    const float offsetX = elapsed * CSF_AT(float, self, 0x88);
    const float offsetY = elapsed * CSF_AT(float, self, 0x8C);
    const float offsetZ = elapsed * CSF_AT(float, self, 0x90);
    const float positionX = offsetX + CSF_AT(float, self, 0x5C);
    const float positionY = offsetY + CSF_AT(float, self, 0x60);
    const float positionZ = offsetZ + CSF_AT(float, self, 0x64);

    CSF_AT(float, self, 0x74) = positionX;
    CSF_AT(float, self, 0x78) = positionY;
    CSF_AT(float, self, 0x7C) = positionZ;

    auto* position = reinterpret_cast<uint8_t*>(self) + 0x74;
    auto* attachedObject = reinterpret_cast<void*>(self->field_0094);
    if (!attachedObject)
        return static_cast<int>(reinterpret_cast<intptr_t>(position));

    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, attachedObject, 0)[0x20 / 4])(
            attachedObject, nullptr, position);

    definition = reinterpret_cast<uint8_t*>(self->field_0018);
    float envelope =
        CSF_AT(float, definition, 0x54) * g_HalfDurationScale;
    if (envelope > self->field_0080)
        envelope = self->field_0080;
    else if (remaining < envelope)
        envelope = remaining;

    const float width = CSF_AT(float, definition, 0x58);
    const float doubledWidth = width + width;
    const float doubledEnvelope = envelope + envelope;

    attachedObject = reinterpret_cast<void*>(self->field_0094);
    return reinterpret_cast<int(__fastcall*)(void*, void*, float, float)>(
        CSF_AT(void**, attachedObject, 0)[0x28 / 4])(
            attachedObject, nullptr, doubledEnvelope, doubledWidth);
}

HOOK(0x006DCC60, C_00861508_UpdateMotion, "auto");
