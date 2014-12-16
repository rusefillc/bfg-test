/**
 * @file	HIP9011.cpp
 * @brief	HIP9011/TPIC8101 driver
 *
 *	pin1	VDD
 *	pin2	GND
 *
 *	pin8	Chip Select - CS
 *	pin11	Slave Data Out - MISO-
 *	pin12	Slave Data In - MOSI
 *	pin13	SPI clock - SCLK
 *
 * http://www.ti.com/lit/ds/symlink/tpic8101.pdf
 * http://www.intersil.com/content/dam/Intersil/documents/an97/an9770.pdf
 * http://e2e.ti.com/cfs-file/__key/telligent-evolution-components-attachments/00-26-01-00-00-42-36-40/TPIC8101-Training.pdf
 *
 *	SPI frequency: 5MHz
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine.h"
#include "settings.h"
#include "pin_repository.h"

#if EFI_HIP_9011

#define HIP9011_CS_PORT GPIOE
#define HIP9011_CS_PIN 11

// 0b01000000
#define SET_PRESCALER_CMD 0x40
// 0b11100000
#define SET_CHANNEL_CMD 0xE0
// 0b00000000
#define SET_BAND_PASS_CMD 0x0

// 0b01110001
//#define HIP_ADVANCED_MODE 0x71


static Logging logger;

static THD_WORKING_AREA(htThreadStack, UTILITY_THREAD_STACK_SIZE);

static int callbackc = 0;

static void spiCallback(SPIDriver *spip) {
	spiUnselectI(spip);

	scheduleMsg(&logger, "spiCallback HIP=%d", callbackc++);

}

// SPI_CR1_BR_1 // 5MHz

static const SPIConfig spicfg = { spiCallback,
/* HW dependent part.*/
HIP9011_CS_PORT,
HIP9011_CS_PIN,
//SPI_CR1_MSTR |
//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };

static unsigned char tx_buff[8];
static unsigned char rx_buff[8];

static SPIDriver *driver = &SPID2;

static msg_t ivThread(int param) {
	chRegSetThreadName("HIP");

	int counter = 0;


	// 4MHz
	tx_buff[0] = SET_PRESCALER_CMD;

	// channel #1
	tx_buff[4] = SET_CHANNEL_CMD;

	while (TRUE) {
		chThdSleepMilliseconds(10);

		scheduleMsg(&logger, "poking HIP=%d", counter++);

		spiSelect(driver);

		spiStartExchange(driver, 2, tx_buff, rx_buff);
		spiUnselect(driver);

	}
#if defined __GNUC__
	return 0;
#endif
}

EXTERN_ENGINE
;

#define INT_TIME_COUNT 32

/**
 * These are HIP9011 magic values - integrator time constants in uS
 */
static const int integratorValues[INT_TIME_COUNT] = { 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 110, 120, 130, 140,
		150, 160, 180, 200, 220, 240, 260, 280, 300, 320, 360, 400, 440, 480, 520, 560, 600 };

#define BAND_LOOKUP_SIZE 64

static const float bandFreqLookup[BAND_LOOKUP_SIZE] = {1.22, 1.26, 1.31, 1.35, 1.4, 1.45, 1.51, 1.57,
		1.63, 1.71, 1.78, 1.87, 1.96, 2.07, 2.18, 2.31, 2.46, 2.54, 2.62, 2.71, 2.81, 2.92, 3.03,
		3.15, 3.28, 3.43, 3.59, 3.76, 3.95, 4.16 , 4.39, 4.66, 4.95, 5.12, 5.29, 5.48, 5.68, 5.9,
		6.12, 6.37, 6.64, 6.94, 7.27, 7.63, 8.02, 8.46, 8.95, 9.5, 10.12, 10.46, 10.83, 11.22, 11.65,
		12.1, 12.6, 13.14, 13.72, 14.36, 15.07, 15.84, 16.71, 17.67, 18.76, 19.98};


#define PIF 3.14159f

static float rpmLookup[INT_TIME_COUNT];

/**
 * 'TC is typically TINT/(2*Pi*VOUT)'
 * Knock Sensor Training TPIC8101, page 24
 *
 * We know the set of possible integration times, we know the knock detection window width
 *
 * 2.2 volts should
 *
 */
#define DESIRED_OUTPUT_VALUE 2.2f
static void prepareRpmLookup(engine_configuration_s *engineConfiguration) {
	for (int i = 0; i < INT_TIME_COUNT; i++) {
		float windowWidthMult = (engineConfiguration->knockDetectionWindowEnd
				- engineConfiguration->knockDetectionWindowStart) / 360.0f;
		// '60000000' because revolutions per MINUTE in uS conversion

		rpmLookup[i] = 60000000.0f / (integratorValues[i] * 2 * PIF * DESIRED_OUTPUT_VALUE * windowWidthMult);
	}
}

#define BAND(bore) (900 / (PIF * (bore) / 2))

static int bandIndex;

static void showHipInfo(void) {
	printSpiState(&logger, boardConfiguration);
	scheduleMsg(&logger, "bore=%f freq=%f", engineConfiguration->cylinderBore, BAND(engineConfiguration->cylinderBore));

	scheduleMsg(&logger, "band index=%d", bandIndex);


	scheduleMsg(&logger, "spi= CS=%s", hwPortname(boardConfiguration->hip9011CsPin));
}

void setHip9011FrankensoPinout(void) {
	/**
	 * SPI on PB13/14/15
	 * ChipSelect is hard-wired
	 */
	boardConfiguration->isHip9011Enabled = true;
	//boardConfiguration->hip9011CsPin =
	boardConfiguration->hip9011IntHoldPin = GPIOB_11;
}

void initHip9011(void) {
	if (!boardConfiguration->isHip9011Enabled)
		return;
	initLogging(&logger, "HIP driver");

	print("Starting HIP9011/TPIC8101 driver\r\n");
	spiStart(driver, &spicfg);

//	chThdCreateStatic(htThreadStack, sizeof(htThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);

	bandIndex = findIndex(bandFreqLookup, BAND_LOOKUP_SIZE, BAND(engineConfiguration->cylinderBore));


	addConsoleAction("hipinfo", showHipInfo);
}

#endif
