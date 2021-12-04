/*
 * @file	trigger_central.h
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_enums.h"
#include "listener_array.h"
#include "trigger_decoder.h"
#include "trigger_central_generated.h"
#include "timer.h"
#include "pin_repository.h"
#include "local_version_holder.h"

#define MAP_CAM_BUFFER 64

class Engine;
typedef void (*ShaftPositionListener)(trigger_event_e signal, uint32_t index, efitick_t edgeTimestamp);

#define HAVE_CAM_INPUT() (isBrainPinValid(engineConfiguration->camInputs[0]))

class TriggerNoiseFilter {
public:
	void resetAccumSignalData();
	bool noiseFilter(efitick_t nowNt,
			TriggerState * triggerState,
			trigger_event_e signal);

	efitick_t lastSignalTimes[HW_EVENT_TYPES];
	efitick_t accumSignalPeriods[HW_EVENT_TYPES];
	efitick_t accumSignalPrevPeriods[HW_EVENT_TYPES];
};

struct MapState {
	float current, previous, prevPrevious;
	cyclic_buffer<float, MAP_CAM_BUFFER> mapBuffer;

	void add(float value) {
		// rotate state
		prevPrevious = previous;
		previous = current;

		// add new value
		mapBuffer.add(value);
		current = mapBuffer.sum(MAP_CAM_BUFFER);
	}

	bool isPeak() {
		return previous > prevPrevious && previous >= current;
	}
};


/**
 * Maybe merge TriggerCentral and TriggerState classes into one class?
 * Probably not: we have an instance of TriggerState which is used for trigger initialization,
 * also composition probably better than inheritance here
 */
class TriggerCentral final : public trigger_central_s {
public:
	TriggerCentral();
	void handleShaftSignal(trigger_event_e signal, efitick_t timestamp);
	int getHwEventCounter(int index) const;
	void resetCounters();
	void validateCamVvtCounters();

	MapState mapState;

	/**
	 * true if a recent configuration change has changed any of the trigger settings which
	 * we have not adjusted for yet
	 */
	bool triggerConfigChanged = false;
	LocalVersionHolder triggerVersion;

	bool checkIfTriggerConfigChanged();
	bool isTriggerConfigChanged();

	bool isTriggerDecoderError();

	expected<float> getCurrentEnginePhase(efitick_t nowNt) const;

	float getTimeSinceTriggerEvent(efitick_t nowNt) const {
		return m_lastEventTimer.getElapsedSeconds(nowNt);
	}

	bool engineMovedRecently() const {
		// Trigger event some time in the past second = engine moving
		// distributor single tooth, large engines crank at close to 120 RPM
		// todo: make this logic account current trigger to stop idle much faster if we have more teeth on trigger wheels?
		return getTimeSinceTriggerEvent(getTimeNowNt()) < 1.0f;
	}

	TriggerNoiseFilter noiseFilter;

	int vvtEventRiseCounter[CAM_INPUTS_COUNT];
	int vvtEventFallCounter[CAM_INPUTS_COUNT];

	trigger_type_e vvtTriggerType[CAMS_PER_BANK];
	angle_t getVVTPosition(uint8_t bankIndex, uint8_t camIndex);

#if EFI_UNIT_TEST
	// latest VVT event position (could be not synchronization event)
	angle_t currentVVTEventPosition[BANKS_COUNT][CAMS_PER_BANK];
#endif // EFI_UNIT_TEST

	// synchronization event position
	angle_t vvtPosition[BANKS_COUNT][CAMS_PER_BANK];

	// todo: convert to Timer!
	efitick_t vvtSyncTimeNt[BANKS_COUNT][CAMS_PER_BANK];

	TriggerStateWithRunningStatistics triggerState;
	TriggerWaveform triggerShape;

	TriggerState vvtState[BANKS_COUNT][CAMS_PER_BANK];
	TriggerWaveform vvtShape[CAMS_PER_BANK];

	TriggerFormDetails triggerFormDetails;

	// Keep track of the last time we got a valid trigger event
	Timer m_lastEventTimer;

private:
	// Keep track of the last time we saw the sync tooth go by (trigger index 0)
	// not TDC point
	Timer m_syncPointTimer;
};

void triggerInfo(void);
void hwHandleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp);
void handleShaftSignal(int signalIndex, bool isRising, efitick_t timestamp);
void hwHandleVvtCamSignal(trigger_value_e front, efitick_t timestamp, int index);

void validateTriggerInputs();

void initTriggerCentral();

int isSignalDecoderError(void);

void onConfigurationChangeTriggerCallback();

#define SYMMETRICAL_CRANK_SENSOR_DIVIDER 4
#define SYMMETRICAL_THREE_TIMES_CRANK_SENSOR_DIVIDER 6
