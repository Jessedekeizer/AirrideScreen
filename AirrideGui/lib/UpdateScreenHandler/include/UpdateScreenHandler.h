#ifndef UPDATESCREENHANDLER_H
#define UPDATESCREENHANDLER_H

#include <lvgl.h>

#include "CanMessageIds.h"
#include "CanMessages.h"
#include "OTACommunication.h"

struct UpdatebleItem {
    int id;
    char name[32];
};

class UpdateScreenHandler {
public:
    static constexpr int MAX_ITEMS = 5;

    explicit UpdateScreenHandler(OTACommunication &ota);

    ~UpdateScreenHandler();

    static UpdateScreenHandler *Active();

    void Begin();

    void OnUpdateSelectScreenLoaded(lv_event_t *e);

    void OnNextUpdateItem(lv_event_t *e);

    void OnPreviousUpdateItem(lv_event_t *e);

    void OnUpdateItemPressed(lv_event_t *e);

    void OnReturnToUpdateSelect(lv_event_t *e);

    void OnUpdateSelectToMain(lv_event_t *e);

    int SelectedId() const { return _selectedId; }

    void SetUpdateProgress(int percent);

    void SetUpdateStatus(const char *status);

private:
    struct ItemSlot {
        lv_obj_t *root;
        lv_obj_t *nameLabel;
        lv_obj_t *button;
    };

    UpdateScreenHandler(const UpdateScreenHandler &) = delete;
    UpdateScreenHandler &operator=(const UpdateScreenHandler &) = delete;

    static constexpr uint32_t SCAN_DURATION_MS = 3000;

    static constexpr uint32_t UPDATE_TIMEOUT_MS = 15000;

    static constexpr uint32_t UPDATE_PUMP_MS = 250;

    static constexpr uint32_t DONE_RESCAN_MS = 5000;

    static constexpr uint32_t START_DELAY_MS = 200;

    void StartScan();

    void OnScanFinished();

    static void ScanTimerCb(lv_timer_t *timer);

    bool AddItem(int id, const char *name);

    void AddNode(ECanNode node);

    void OnOtaStatus(ECanNode node, const CANAirRideOTAStatus &status);

    void BeginUpdate();

    static void StartTimerCb(lv_timer_t *timer);

    void StopUpdate();

    static void UpdatePumpCb(lv_timer_t *timer);

    static void DoneRescanCb(lv_timer_t *timer);

    void CancelDoneRescan();

    void ShowStatus();

    void ShowRetry(bool retry);

    static const char *NodeName(ECanNode node);

    static ItemSlot SlotAt(int index);

    static int SlotIndexForObject(lv_obj_t *obj);

    void PrepareContainer();

    static void ContainerScrollEndCb(lv_event_t *e);

    void ApplyItemsToSlots();

    void ShowSelectedOnUpdateScreen();

    void HideSelectList();

    void RefreshNavButtons();

    void Refresh();

    void SetLoadingVisible(bool visible);

    void ScheduleLoadingReveal();

    void CancelLoadingReveal();

    static void LoadingRevealCb(lv_timer_t *timer);

    static constexpr uint32_t LOADING_REVEAL_MS = 250;

    static UpdateScreenHandler *_active;

    OTACommunication &_ota;

    lv_timer_t *_loadingRevealTimer = nullptr;

    UpdatebleItem _items[MAX_ITEMS] = {};
    int _itemCount = 0;
    int _itemIndex = 0;
    int _selectedId = -1;
    int _selectedIndex = -1;
    bool _scanning = false;
    bool _containerReady = false;
    lv_timer_t *_scanTimer = nullptr;

    bool _updating = false;
    lv_timer_t *_updateTimer = nullptr;
    lv_timer_t *_startTimer = nullptr;
    lv_timer_t *_doneTimer = nullptr;
    uint32_t _lastStatusMs = 0;
    CANAirRideOTAStatus _lastStatus = {};
};

#endif
