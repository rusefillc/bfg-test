// auto-generated by PinoutLogic.java

#include "pch.h"

const char * getBoardSpecificPinName(brain_pin_e brainPin) {
	switch(brainPin) {
		case EFI_ADC_0: return "14 - O2";
		case GPIOC_1: return "101 - MAP2";
		case EFI_ADC_5: return "106 - KNOCK";
		case EFI_ADC_4: return "92 - TPS 1";
		case EFI_ADC_3: return "34 In PPS1";
		case GPIOC_9: return "18 - VVT2 B1";
		case GPIOI_2: return "Main Relay";
		case GPIOG_4: return "47 - CEL";
		case GPIOC_8: return "120 - VVT2 B2";
		case GPIOE_5: return "7 - IGN_2";
		case GPIOA_9: return "104 Wastegate";
		case GPIOG_3: return "115 - VVT1_2";
		case GPIOC_7: return "118 ETB_OUT-";
		case GPIOI_0: return "22 - VVT1 1";
		case GPIOE_4: return "111 - IGN_3";
		case GPIOC_6: return "117 ETB_OUT+";
		case GPIOA_7: return "87 - CAM2";
		case GPIOE_3: return "94 - IGN_4";
		case GPIOA_6: return "86 - CAM1";
		case GPIOE_2: return "103 - IGN_5";
		case GPIOI_8: return "102 - IGN_1";
		case GPIOI_7: return "95 - IGN_8";
		case GPIOI_6: return "8 - IGN_7";
		case GPIOG_8: return "89 - INJ_2";
		case GPIOG_7: return "96 - INJ_1";
		case GPIOI_5: return "110 - IGN_6";
		case GPIOD_10: return "88 - INJ_4";
		case GPIOH_14: return "65 - Fuel Pump";
		case GPIOD_11: return "97 - INJ_3";
		case GPIOD_14: return "105 - IDLE";
		case GPIOD_13: return "37 - TACH";
		case GPIOB_1: return "82 - VR";
		case GPIOD_9: return "113 INJ 5";
		case GPIOF_9: return "48 - IN RES1";
		case GPIOE_13: return "In D2";
		case GPIOE_12: return "In D1";
		case GPIOE_15: return "In D4";
		case GPIOE_14: return "In D3";
		case EFI_ADC_10: return "29 In Maf";
		case EFI_ADC_11: return "101 - MAP2";
		case EFI_ADC_12: return "93 - CLT";
		case EFI_ADC_13: return "85 - IAT";
		case EFI_ADC_14: return "35 In PPS2";
		case ADC3_CHANNEL_7: return "48 - IN RES1";
		case GPIOF_10: return "39 - IN RES2";
		case GPIOF_13: return "19 - INJ 7";
		case GPIOF_14: return "24 - INJ 8";
		case EFI_ADC_8: return "84 - TPS2";
		case GPIOF_11: return "54 - VSS";
		case EFI_ADC_7: return "87 - CAM2";
		case EFI_ADC_6: return "86 - CAM1";
		case GPIOF_12: return "112 INJ 6";
	}
	return nullptr;
}
