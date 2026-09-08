/**
 * @file MainScreenHandler.cpp
 * @brief The main screen: four momentary controls, two pushes, two readings.
 */

#include "MainScreenHandler.h"

#include <math.h>

#include "Logger.h"
#include "actions.h"
#include "vars.h"

MainScreenHandler *MainScreenHandler::_active = nullptr;

/**
 * @brief Bind the readings, the bus and the settings, and become Active.
 */
MainScreenHandler::MainScreenHandler(MainScreenData &data, MainScreenCommunication &communication,
                                     SettingsDevice &settings)
    : _data(data), _communication(communication), _settings(settings) {
    _active = this;
}

/**
 * @brief Cancel a pending auto ride and deregister.
 */
MainScreenHandler::~MainScreenHandler() {
    CancelAutoRide();
    if (_active == this) {
        _active = nullptr;
    }
}

/**
 * @brief The handler the var and action shims talk to.
 * @return The Active handler, or null before one exists.
 */
MainScreenHandler *MainScreenHandler::Active() {
    return _active;
}

/**
 * @brief Push the settings to the controller and arm the auto ride.
 * @param e Unused.
 * @note Runs on every entry, so a Save on the settings screen reaches the
 *       controller on the way back without anything else noticing.
 */
void MainScreenHandler::OnMainScreenLoaded(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Main screen loaded, front/back", _data.front, _data.back);
    _communication.SendSettings();
    ArmAutoRideTimer();
}

/**
 * @brief Front up pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontUpPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front up pressed");
    SendButton(FRONT_UP, true);
}

/**
 * @brief Front up released.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontUpReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front up released");
    SendButton(FRONT_UP, false);
}

/**
 * @brief Front down pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontDownPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front down pressed");
    SendButton(FRONT_DOWN, true);
}

/**
 * @brief Front down released.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontDownReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front down released");
    SendButton(FRONT_DOWN, false);
}

/**
 * @brief Back up pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnBackUpPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back up pressed");
    SendButton(BACK_UP, true);
}

/**
 * @brief Back up released.
 * @param e Unused.
 */
void MainScreenHandler::OnBackUpReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back up released");
    SendButton(BACK_UP, false);
}

/**
 * @brief Back down pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnBackDownPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back down pressed");
    SendButton(BACK_DOWN, true);
}

/**
 * @brief Back down released.
 * @param e Unused.
 */
void MainScreenHandler::OnBackDownReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back down released");
    SendButton(BACK_DOWN, false);
}

/**
 * @brief Park pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnParkPressed(lv_event_t *e) {
    (void) e;
    LOG_INFO("Park pressed");
    SendButton(PARK, true);
}

/**
 * @brief Ride pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnRidePressed(lv_event_t *e) {
    (void) e;
    LOG_INFO("Ride pressed");
    SendButton(RIDE, true);
}

/**
 * @brief Send one control frame and cancel the automatic lift.
 * @param button Which control was used.
 * @param state  true while held, false on release.
 */
void MainScreenHandler::SendButton(EMainScreenButtons button, bool state) {
    CancelAutoRide();
    _communication.SendMessageButtonPress(button, state);
}

/**
 * @brief Arm the one-shot automatic lift, if the setting asks for it.
 * @note Re-entering the main screen does not re-arm it; the lift is meant
 *       to happen once, shortly after the car is switched on.
 */
void MainScreenHandler::ArmAutoRideTimer() {
    if (!_settings.autoRide) {
        LOG_DEBUG("Auto start ride is disabled");
        return;
    }
    if (_abortAutoRide) {
        LOG_DEBUG("Auto start ride already run or cancelled");
        return;
    }
    if (_autoRideTimer != nullptr) {
        LOG_DEBUG("Auto ride timer already armed");
        return;
    }

    const uint32_t delayMs = (uint32_t) (_settings.autoRideSec * 1000.0f);
    _autoRideTimer = lv_timer_create(AutoRideTimerCb, delayMs, this);
    if (_autoRideTimer == nullptr) {
        LOG_ERROR("Could not create the auto ride timer");
        return;
    }
    lv_timer_set_repeat_count(_autoRideTimer, 1);
    LOG_DEBUG("Auto ride timer armed for ms", delayMs);
}

/**
 * @brief Drop a pending automatic lift and stop a later one being armed.
 */
void MainScreenHandler::CancelAutoRide() {
    _abortAutoRide = true;
    if (_autoRideTimer != nullptr) {
        lv_timer_delete(_autoRideTimer);
        _autoRideTimer = nullptr;
    }
}

/**
 * @brief Timer callback for the automatic lift.
 * @param timer Carries the handler.
 */
void MainScreenHandler::AutoRideTimerCb(lv_timer_t *timer) {
    MainScreenHandler *self = (MainScreenHandler *) lv_timer_get_user_data(timer);
    self->_autoRideTimer = nullptr;
    self->AutoStartRide();
}

/**
 * @brief Lift the car unless it is already up, or the user intervened.
 */
void MainScreenHandler::AutoStartRide() {
    if (_abortAutoRide) {
        return;
    }
    _abortAutoRide = true;

    if (_data.front < AUTO_RIDE_PRESSURE_THRESHOLD || _data.back < AUTO_RIDE_PRESSURE_THRESHOLD) {
        LOG_INFO("Auto ride: car is down, sending ride command");
        _communication.SendMessageButtonPress(RIDE, true);
    } else {
        LOG_INFO("Auto ride: car is already up, nothing to do");
    }
}

/**
 * @brief Hand in fresh sensor readings. Safe to call at any rate.
 * @param front Front reading, or NaN to leave it alone.
 * @param back  Back reading, or NaN to leave it alone.
 */
void MainScreenHandler::UpdatePressures(float front, float back) {
    if (!isnan(front)) {
        _data.front = front;
    }
    if (!isnan(back)) {
        _data.back = back;
    }
}

/**
 * @brief Read front pressure for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_front_pressure() {
    MainScreenHandler *handler = MainScreenHandler::Active();
    return handler != nullptr ? handler->FrontPressure() : 0.0f;
}

/**
 * @brief Write front pressure from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_front_pressure(float value) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->UpdatePressures(value, NAN);
    }
}

/**
 * @brief Read back pressure for the flow engine.
 * @return The current value.
 */
extern "C" float get_var_back_pressure() {
    MainScreenHandler *handler = MainScreenHandler::Active();
    return handler != nullptr ? handler->BackPressure() : 0.0f;
}

/**
 * @brief Write back pressure from the flow engine.
 * @param value New value.
 */
extern "C" void set_var_back_pressure(float value) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->UpdatePressures(NAN, value);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_main_screen_loaded(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnMainScreenLoaded(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_up_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontUpPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_up_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontUpReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_down_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontDownPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_down_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontDownReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_up_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackUpPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_up_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackUpReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_down_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackDownPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_down_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackDownReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_park_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnParkPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_ride_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnRidePressed(e);
    }
}
