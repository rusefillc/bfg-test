ifeq ($(KINETIS_CONTRIB),)
  KINETIS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_ADC TRUE,$(HALCONF)),)
PLATFORMSRC_CONTRIB += ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/ADCv2/hal_adc_lld.c
endif
else
PLATFORMSRC_CONTRIB += ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/ADCv2/hal_adc_lld.c
endif

PLATFORMINC_CONTRIB += ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/ADCv2
