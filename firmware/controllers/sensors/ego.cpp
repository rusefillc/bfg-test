/**
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 * EGO Exhaust Gas Oxygen, also known as AFR Air/Fuel Ratio :)
 *
 * At the moment linear external widebands are supported, also there is a 8-point interpolation curve to emulate a wide-band with a narrow-band sensor.
 *
 * rusEfi does not have it's own wide-band controller as of Nov 2017
 *
 */
#include "ego.h"
#include "interpolation.h"
#include "engine.h"
#include "analog_input.h"

EXTERN_ENGINE;

bool hasAfrSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->afr.hwChannel != EFI_ADC_NONE;
}

float getAfr(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	afr_sensor_s * sensor = &CONFIG(afr);

	float volts = getVoltageDivided("ego", sensor->hwChannel);

	if (boardConfiguration->afr_type == ES_NarrowBand) {
		return interpolate2d("narrow", volts, engineConfiguration->narrowToWideOxygenBins, engineConfiguration->narrowToWideOxygen, NARROW_BAND_WIDE_BAND_CONVERSION_SIZE);
	}

	return interpolate(sensor->v1, sensor->value1, sensor->v2, sensor->value2, volts)
			+ engineConfiguration->egoValueShift;
}

static void initEgoSensor(afr_sensor_s *sensor, ego_sensor_e type) {

	switch (type) {
	case ES_BPSX_D1:
		/**
		 * This decodes BPSX D1 Wideband Controller analog signal
		 */
		sensor->v1 = 0;
		sensor->value1 = 9;
		sensor->v2 = 5;
		sensor->value2 = 19;
		break;

	case ES_Innovate_MTX_L:
		sensor->v1 = 0;
		sensor->value1 = 7.35;
		sensor->v2 = 5;
		sensor->value2 = 22.39;
		break;
	case ES_14Point7_Free:
		sensor->v1 = 0;
		sensor->value1 = 9.996;
		sensor->v2 = 5;
		sensor->value2 = 19.992;
		break;
		// technically 14Point7 and PLX use the same scale
	case ES_PLX:
		sensor->v1 = 0;
		sensor->value1 = 10;
		sensor->v2 = 5;
		sensor->value2 = 20;
		break;
	case ES_NarrowBand:
		sensor->v1 = 0.1;
		sensor->value1 = 15;
		sensor->v2 = 0.9;
		sensor->value2 = 14;
		break;
	default:
		firmwareError(CUSTOM_EGO_TYPE, "Unexpected EGO %d", type);
		break;
	}
}

void setEgoSensor(ego_sensor_e type DECLARE_ENGINE_PARAMETER_SUFFIX) {
	boardConfiguration->afr_type = type;
	initEgoSensor(&engineConfiguration->afr, type);
}
