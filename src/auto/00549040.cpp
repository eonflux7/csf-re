#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable byte offset of the setter on the widget objects (slot 0x2A8 / 4 = 170).
static constexpr size_t kWidgetSetValueOffset = 0x2A8;

// Reads the int returned by a getter slot on the settings object (this+0x24) and passes it
// to the setter slot of the widget stored in *widgetField. Load order follows the original:
// settings pointer, widget pointer, settings vtable, widget vtable, then the getter call,
// after which the widget pointer is re-read before the setter call.
static int ForwardGetterValueToWidget(CMenuVideoAdvanced* self, uint32_t* widgetField, size_t getterSlotOffset)
{
    void* settings = reinterpret_cast<void*>(self->field_0024);
    void* widget = reinterpret_cast<void*>(*widgetField);
    void** settingsVtbl = CSF_AT(void**, settings, 0);
    void** widgetVtbl = CSF_AT(void**, widget, 0);

    int value = reinterpret_cast<int(__fastcall*)(void*, void*)>(settingsVtbl[getterSlotOffset / 4])(settings, nullptr);

    widget = reinterpret_cast<void*>(*widgetField);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(widgetVtbl[kWidgetSetValueOffset / 4])(widget, nullptr, value);
}

// Same load order as ForwardGetterValueToWidget, but the getter returns a byte flag (al)
// and the widget receives 1 when that flag is zero, otherwise 0.
static int ForwardInvertedFlagToWidget(CMenuVideoAdvanced* self, uint32_t* widgetField)
{
    void* settings = reinterpret_cast<void*>(self->field_0024);
    void* widget = reinterpret_cast<void*>(*widgetField);
    void** settingsVtbl = CSF_AT(void**, settings, 0);
    void** widgetVtbl = CSF_AT(void**, widget, 0);

    uint8_t flag = reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(settingsVtbl[0x38 / 4])(settings, nullptr);
    int value = (flag == 0) ? 1 : 0;

    widget = reinterpret_cast<void*>(*widgetField);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(widgetVtbl[kWidgetSetValueOffset / 4])(widget, nullptr, value);
}

// Pushes the current advanced video settings into the eight option widgets of this screen.
// Returns 0 if the settings source (+0x50) or any widget pointer (+0x28..+0x44) is null;
// otherwise returns the value returned by the last widget setter.
static int __fastcall CMenuVideoAdvanced_PushSettingsToWidgets(CMenuVideoAdvanced* self, void* /*edx*/)
{
    if (!self->field_0050) return 0;
    if (!self->field_0028) return 0;
    if (!self->field_002c) return 0;
    if (!self->field_0030) return 0;
    if (!self->field_0034) return 0;
    if (!self->field_0038) return 0;
    if (!self->field_003c) return 0;
    if (!self->field_0040) return 0;
    if (!self->field_0044) return 0;

    ForwardInvertedFlagToWidget(self, &self->field_0028);
    ForwardGetterValueToWidget(self, &self->field_002c, 0x40);
    ForwardGetterValueToWidget(self, &self->field_0030, 0x48);
    ForwardGetterValueToWidget(self, &self->field_0034, 0x50);
    ForwardGetterValueToWidget(self, &self->field_0038, 0x58);
    ForwardGetterValueToWidget(self, &self->field_003c, 0x60);
    ForwardGetterValueToWidget(self, &self->field_0040, 0x68);
    return ForwardGetterValueToWidget(self, &self->field_0044, 0x70);
}

HOOK(0x00549040, CMenuVideoAdvanced_PushSettingsToWidgets, "auto");
