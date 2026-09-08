#include "SettingsScreenHandler.h"

#include <stdint.h>
#include "Logger.h"
#include "actions.h"
#include "screens.h"
#include "vars.h"

SettingsScreenHandler *SettingsScreenHandler::_active = nullptr;

extern "C" void (*tick_screen_funcs[])();

void (*SettingsScreenHandler::_generatedSettingsTick)() = nullptr;
bool SettingsScreenHandler::_settingsDirty = true;
uint32_t SettingsScreenHandler::_lastFullTickMs = 0;

/**
 * @brief Note that something a settings screen shows has changed.
 */
void SettingsScreenHandler::MarkDirty() {
    _settingsDirty = true;
}

/**
 * @brief Stands in for the generated settings tick.
 * @note The generated one re-evaluates 55 bindings and strcmps each result
 *       against the label. This runs it only when something changed, or
 *       when the backstop is due.
 */
void SettingsScreenHandler::SettingsScreenTickCb() {
    if (_generatedSettingsTick == nullptr) {
        return;
    }

    const uint32_t now = lv_tick_get();
    const bool refreshDue = (now - _lastFullTickMs) >= TICK_REFRESH_MS;

    if (!_settingsDirty && !refreshDue) {
        return;
    }

    _settingsDirty = false;
    _lastFullTickMs = now;
    _generatedSettingsTick();
}

/**
 * @brief Install the tick filter, keeping the generated one to call.
 */
void SettingsScreenHandler::InstallTickFilter() {
    if (_generatedSettingsTick != nullptr) {
        return;
    }

    const int index = SCREEN_ID_SETTINGS_SELECT_SCREEN - 1;
    _generatedSettingsTick = tick_screen_funcs[index];
    tick_screen_funcs[index] = SettingsScreenTickCb;
}

/**
 * @brief Bind the settings and their storage, and become Active.
 */
SettingsScreenHandler::SettingsScreenHandler(SettingsDevice &settings, SettingsStorage &storage)
    : _settings(settings), _storage(storage) {
    if (_active != nullptr) {
        LOG_WARN("Second SettingsScreenHandler built, replacing the Active one");
    }
    _active = this;
}

/**
 * @brief Drop a pending page refresh and deregister.
 */
SettingsScreenHandler::~SettingsScreenHandler() {
    CancelPageRefresh();
    if (_active == this) {
        _active = nullptr;
    }
}

/**
 * @brief The handler the var and action shims talk to.
 * @return The Active handler, or null before one exists.
 */
SettingsScreenHandler *SettingsScreenHandler::Active() {
    return _active;
}

/**
 * @brief Write the settings to the card.
 * @param e Unused.
 */
void SettingsScreenHandler::OnSaveSettingsPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Save settings pressed");
    Save();
}

/**
 * @brief The page container for an index.
 * @param index Page index.
 * @return The container, or null when out of range.
 */
lv_obj_t *SettingsScreenHandler::PageAt(int index) {
    switch (index) {
        case 0:
            return objects.settings1_item;
        case 1:
            return objects.settings2_item;
        case 2:
            return objects.settings3_item;
        case 3:
            return objects.settings4_item;
        default:
            return nullptr;
    }
}

/**
 * @brief The container the four pages sit in, side by side.
 * @return The container.
 */
lv_obj_t *SettingsScreenHandler::PagerContainer() {
    return objects.settings_item_container;
}

/**
 * @brief Width of one page, which is also the scroll step between them.
 * @return The stride in pixels, or 0 if nothing can be measured.
 */
int32_t SettingsScreenHandler::PageStride() {
    lv_obj_t *cont = PagerContainer();
    if (cont != nullptr) {
        lv_obj_update_layout(cont);
    }

    lv_obj_t *page = PageAt(0);
    if (page != nullptr) {
        const int32_t w = lv_obj_get_width(page);
        if (w > 0) {
            return w;
        }
    }

    if (cont != nullptr) {
        const int32_t w = lv_obj_get_width(cont);
        if (w > 0) {
            return w;
        }
    }

    lv_display_t *disp = lv_display_get_default();
    return disp != nullptr ? lv_display_get_horizontal_resolution(disp) : 0;
}

/**
 * @brief Schedule a scroll to the current page.
 * @warning The scroll is deferred rather than done here. Every caller is
 *          inside an LVGL event and lv_obj_scroll_to_x() dispatches events
 *          of its own, which crashed LVGL three levels deep.
 */
void SettingsScreenHandler::RefreshPage() {
    InstallTickFilter();

    if (_pageRefreshTimer != nullptr) {
        return;
    }
    _pageRefreshTimer = lv_timer_create(PageRefreshCb, 0, this);
    lv_timer_set_repeat_count(_pageRefreshTimer, 1);
}

/**
 * @brief Drop a pending page refresh.
 */
void SettingsScreenHandler::CancelPageRefresh() {
    if (_pageRefreshTimer != nullptr) {
        lv_timer_delete(_pageRefreshTimer);
        _pageRefreshTimer = nullptr;
    }
}

/**
 * @brief Timer callback that performs the deferred scroll.
 * @param timer Carries the handler.
 */
void SettingsScreenHandler::PageRefreshCb(lv_timer_t *timer) {
    SettingsScreenHandler *self = (SettingsScreenHandler *) lv_timer_get_user_data(timer);
    self->_pageRefreshTimer = nullptr;
    self->ApplyPage();
}

/**
 * @brief Scroll the pager to the current page, without animating.
 */
void SettingsScreenHandler::ApplyPage() {
    lv_obj_t *cont = PagerContainer();
    const int32_t stride = PageStride();
    if (cont == nullptr || stride <= 0) {
        LOG_WARN("Cannot page: container", cont != nullptr, "stride", stride);
        return;
    }

    lv_obj_scroll_to_x(cont, _pageIndex * stride, LV_ANIM_OFF);
}

/**
 * @brief Back one settings page.
 * @param e Unused.
 */
void SettingsScreenHandler::OnSettingsLeftPressed(lv_event_t *e) {
    (void) e;
    if (_pageIndex <= 0) {
        LOG_DEBUG("Already at settings1, cannot go back");
        return;
    }
    _pageIndex--;
    LOG_DEBUG("Back to settings", _pageIndex + 1);
    RefreshPage();
}

/**
 * @brief Forward one settings page.
 * @param e Unused.
 */
void SettingsScreenHandler::OnSettingsRightPressed(lv_event_t *e) {
    (void) e;
    if (_pageIndex + 1 >= PAGE_COUNT) {
        LOG_DEBUG("Already at settings", PAGE_COUNT, "cannot go on");
        return;
    }
    _pageIndex++;
    LOG_DEBUG("Forward to settings", _pageIndex + 1);
    RefreshPage();
}

/**
 * @brief Reload from the card and start at page one.
 * @param e Unused.
 * @note Hooked to the screen load rather than the button, so it runs
 *       however the screen is reached and nothing stale is drawn.
 */
void SettingsScreenHandler::OnSettingsSelectLoadStart(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Settings screen loaded, reloading from card");
    Load();

    InstallTickFilter();
    _pageIndex = 0;
    RefreshPage();
}

/**
 * @brief On the way to the settings screen.
 * @param e Unused.
 */
void SettingsScreenHandler::OnMainScreenToSettingsSelectPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Main screen to settings pressed");
}

/**
 * @brief On the way to the update screens.
 * @param e Unused.
 */
void SettingsScreenHandler::OnSettingsToUpdatePressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Settings to update pressed");
}

/**
 * @brief Read the card into the settings.
 */
void SettingsScreenHandler::Load() {
    _storage.ReadSettings(_settings);
    MarkDirty();
}

/**
 * @brief Write the settings to the card, off this thread.
 */
void SettingsScreenHandler::Save() {
    _storage.WriteSettingsAsync(_settings);
}

/**
 * @brief The struct the variable shims read and write.
 * @return The settings, or null when no handler exists yet.
 */
static SettingsDevice *ActiveSettings() {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings var touched with no handler");
        return nullptr;
    }
    return &handler->Settings();
}

/**
 * @brief Read front max for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_front_max() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->frontMax : 0.0f;
}

/**
 * @brief Write front max from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_front_max(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->frontMax = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read back max for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_back_max() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->backMax : 0.0f;
}

/**
 * @brief Write back max from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_back_max(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->backMax = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read ride front for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_ride_front() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->rideFront : 0.0f;
}

/**
 * @brief Write ride front from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_ride_front(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->rideFront = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read ride back for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_ride_back() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->rideBack : 0.0f;
}

/**
 * @brief Write ride back from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_ride_back(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->rideBack = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read front up x for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_front_up_x() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->frontUpX : 0.0f;
}

/**
 * @brief Write front up x from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_front_up_x(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->frontUpX = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read front down x for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_front_down_x() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->frontDownX : 0.0f;
}

/**
 * @brief Write front down x from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_front_down_x(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->frontDownX = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read back up x for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_back_up_x() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->backUpX : 0.0f;
}

/**
 * @brief Write back up x from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_back_up_x(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->backUpX = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read back down x for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_back_down_x() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->backDownX : 0.0f;
}

/**
 * @brief Write back down x from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_back_down_x(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->backDownX = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read auto ride for the flow engine.
 * @return The current value.
 */
extern "C" bool get_var_auto_ride() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->autoRide : false;
}

/**
 * @brief Write auto ride from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_auto_ride(bool value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->autoRide = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read auto park for the flow engine.
 * @return The current value.
 */
extern "C" bool get_var_auto_park() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->autoPark : false;
}

/**
 * @brief Write auto park from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_auto_park(bool value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->autoPark = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read auto ride sec for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_auto_ride_sec() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->autoRideSec : 0.0f;
}

/**
 * @brief Write auto ride sec from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_auto_ride_sec(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->autoRideSec = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read auto park sec for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_auto_park_sec() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->autoParkSec : 0.0f;
}

/**
 * @brief Write auto park sec from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_auto_park_sec(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->autoParkSec = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read logging for the flow engine.
 * @return The current value.
 */
extern "C" bool get_var_logging() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->logging : false;
}

/**
 * @brief Write logging from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_logging(bool value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->logging = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read machine learning for the flow engine.
 * @return The current value.
 */
extern "C" bool get_var_machine_learning() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->machineLearning : false;
}

/**
 * @brief Write machine learning from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_machine_learning(bool value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->machineLearning = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read bluetooth for the flow engine.
 * @return The current value.
 */
extern "C" bool get_var_bluetooth() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->bluetooth : false;
}

/**
 * @brief Write bluetooth from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_bluetooth(bool value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->bluetooth = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Read park duration sec for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_park_duration_sec() {
    SettingsDevice *s = ActiveSettings();
    return s != nullptr ? s->parkDuration : 0.0f;
}

/**
 * @brief Write park duration sec from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_park_duration_sec(float value) {
    SettingsDevice *s = ActiveSettings();
    if (s != nullptr) {
        s->parkDuration = value;
        SettingsScreenHandler::MarkDirty();
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_save_settings_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Save settings pressed with no handler");
        return;
    }
    handler->OnSaveSettingsPressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_settings_to_update_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings to update pressed with no handler");
        return;
    }
    handler->OnSettingsToUpdatePressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_main_screen_to_settings_select_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Main screen to settings pressed with no handler");
        return;
    }
    handler->OnMainScreenToSettingsSelectPressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_settings_left_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings left pressed with no handler");
        return;
    }
    handler->OnSettingsLeftPressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_settings_right_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings right pressed with no handler");
        return;
    }
    handler->OnSettingsRightPressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_settings_select_load_start(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings screen loaded with no handler");
        return;
    }
    handler->OnSettingsSelectLoadStart(e);
}
