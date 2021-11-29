#include "hwbp_core.h"
#include "hwbp_core_regs.h"
#include "hwbp_core_types.h"

#include "app.h"
#include "app_funcs.h"
#include "app_ios_and_regs.h"

#define F_CPU 32000000
#include <util/delay.h>

/************************************************************************/
/* Declare application registers                                        */
/************************************************************************/
extern AppRegs app_regs;
extern uint8_t app_regs_type[];
extern uint16_t app_regs_n_elements[];
extern uint8_t *app_regs_pointer[];
extern void (*app_func_rd_pointer[])(void);
extern bool (*app_func_wr_pointer[])(void*);

/************************************************************************/
/* Initialize app                                                       */
/************************************************************************/
static const uint8_t default_device_name[] = "CameraControllerGen2";

void hwbp_app_initialize(void)
{
    /* Define versions */
    uint8_t hwH = 1;
    uint8_t hwL = 0;
    uint8_t fwH = 0;
    uint8_t fwL = 1;
    uint8_t ass = 0;
    
   	/* Start core */
    core_func_start_core(
        1170,
        hwH, hwL,
        fwH, fwL,
        ass,
        (uint8_t*)(&app_regs),
        APP_NBYTES_OF_REG_BANK,
        APP_REGS_ADD_MAX - APP_REGS_ADD_MIN + 1,
        default_device_name
    );
}

/************************************************************************/
/* Handle if a catastrophic error occur                                 */
/************************************************************************/
void core_callback_catastrophic_error_detected(void)
{
	clr_LED_CAM0;
	clr_LED_CAM1;
	
	timer_type0_stop(&TCD0);
	timer_type0_stop(&TCC0);
	clr_LED_CAM0;
	clr_LED_CAM1;
}

/************************************************************************/
/* User functions                                                       */
/************************************************************************/
/* Add your functions here or load external functions if needed */

/************************************************************************/
/* Initialization Callbacks                                             */
/************************************************************************/
#define T_STARTUP_ON  50
#define T_STARTUP_OFF 0

void core_callback_1st_config_hw_after_boot(void)
{
	/* Initialize IOs */
	/* Don't delete this function!!! */
	init_ios();
	
	/* Initialize hardware */
	for (uint8_t i = 0; i < 2; i++)
	{
		set_LED_CAM0;  _delay_ms(T_STARTUP_ON); clr_LED_CAM0;  _delay_ms(T_STARTUP_OFF);
		set_LED_PWR;  _delay_ms(T_STARTUP_ON); clr_LED_PWR;  _delay_ms(T_STARTUP_OFF);
		set_LED_CAM1;  _delay_ms(T_STARTUP_ON); clr_LED_CAM1;  _delay_ms(T_STARTUP_OFF);
		set_LED_STATE;  _delay_ms(T_STARTUP_ON); clr_LED_STATE;  _delay_ms(T_STARTUP_OFF);
	}
	_delay_ms(T_STARTUP_ON*2);

	for (uint8_t i = 0; i < 2; i++)
	{
		set_LED_CAM0; set_LED_PWR;
		set_LED_CAM1; set_LED_STATE;
		_delay_ms(T_STARTUP_ON*2);
			
		clr_LED_CAM0; clr_LED_PWR;
		clr_LED_CAM1; clr_LED_STATE;
		_delay_ms(T_STARTUP_ON*2);
	}
		
	_delay_ms(500);
	set_LED_PWR;
}

void core_callback_reset_registers(void)
{
	/* Initialize registers */
	
	//app_regs.REG_CAM0_EVENT;
	//app_regs.REG_CAM1_EVENT;
	
	app_regs.REG_CAM0_EVENT_CONFIG = MSK_EVENT_ON_STROBE;
	app_regs.REG_CAM1_EVENT_CONFIG = MSK_EVENT_ON_STROBE;
	
	app_regs.REG_START_AND_STOP = B_STOP_CAM0 | B_STOP_CAM1;
	app_regs.REG_START_TIMESTAMP = 0;
	app_regs.REG_STOP_TIMESTAMP = 0;
	
	app_regs.REG_RESERVED0 = 0;
	app_regs.REG_RESERVED1 = 0;
	
	app_regs.REG_CAM0_TRIGGER_CONFIG = MSK_TRG_SRC_INTERNAL_50Hz;
	app_regs.REG_CAM0_TRIGGER_INVERTED = MSK_TRIGGER_INVERTED_NO;
	app_regs.REG_CAM0_STROBE_SOURCE = MSK_STROBE_PULL_UP;
	app_regs.REG_CAM0_TRIGGER_FREQUENCY = 50;		// 50 Hz
	app_regs.REG_CAM0_TRIGGER_DURATION_US = 1000;	// 1 mS
	
	app_regs.REG_RESERVED2 = 0;
	app_regs.REG_RESERVED3 = 0;
	
	app_regs.REG_CAM1_TRIGGER_CONFIG = MSK_TRG_SRC_INTERNAL_125Hz;
	app_regs.REG_CAM1_TRIGGER_INVERTED = MSK_TRIGGER_INVERTED_NO;
	app_regs.REG_CAM1_STROBE_SOURCE = MSK_STROBE_PULL_UP;
	app_regs.REG_CAM1_TRIGGER_FREQUENCY = 125;		// 125 Hz
	app_regs.REG_CAM1_TRIGGER_DURATION_US = 1000;	// 1 ms
	
	app_regs.REG_RESERVED4 = 0;
	app_regs.REG_RESERVED5 = 0;
	
	app_regs.REG_OUT0_CONFIG = MSK_OUT0_TRIGGER_CAM0;
	app_regs.REG_OUT1_CONFIG = MSK_OUT1_TRIGGER_CAM1;
	
	app_regs.REG_RESERVED6 = 0;
	app_regs.REG_RESERVED7 = 0;
	
	//app_regs.REG_OUT_SET;
	//app_regs.REG_OUT_CLEAR;
	//app_regs.REG_OUT_TOGGLE;
	app_regs.REG_OUT_WRITE = 0;
	
	//app_regs.REG_IN_STATE;
	
	
	
	//app_regs.REG_CAM0_EVENT;
	//app_regs.REG_CAM1_EVENT;
	
	app_regs.REG_CAM0_EVENT_CONFIG = MSK_EVENT_ON_TRIGGER;
	app_regs.REG_CAM1_EVENT_CONFIG = MSK_EVENT_ON_TRIGGER;
	
	app_regs.REG_START_AND_STOP = B_STOP_CAM0 | B_STOP_CAM1;
	app_regs.REG_START_TIMESTAMP = 1;
	app_regs.REG_STOP_TIMESTAMP = 5;
	
	app_regs.REG_RESERVED0 = 0;
	app_regs.REG_RESERVED1 = 0;
	
	app_regs.REG_CAM0_TRIGGER_CONFIG = MSK_TRG_SRC_INTERNAL;
	app_regs.REG_CAM0_TRIGGER_INVERTED = MSK_TRIGGER_INVERTED_NO;
	app_regs.REG_CAM0_STROBE_SOURCE = MSK_STROBE_PULL_UP;
	app_regs.REG_CAM0_TRIGGER_FREQUENCY = 10;		// 50 Hz
	app_regs.REG_CAM0_TRIGGER_DURATION_US = 1000;	// 1 mS
	
	app_regs.REG_RESERVED2 = 0;
	app_regs.REG_RESERVED3 = 0;
	
	app_regs.REG_CAM1_TRIGGER_CONFIG = MSK_TRG_SRC_INTERNAL;
	app_regs.REG_CAM1_TRIGGER_INVERTED = MSK_TRIGGER_INVERTED_NO;
	app_regs.REG_CAM1_STROBE_SOURCE = MSK_STROBE_PULL_UP;
	app_regs.REG_CAM1_TRIGGER_FREQUENCY = 10;		// 125 Hz
	app_regs.REG_CAM1_TRIGGER_DURATION_US = 1000;	// 1 ms
	
	app_regs.REG_RESERVED4 = 0;
	app_regs.REG_RESERVED5 = 0;
	
	app_regs.REG_OUT0_CONFIG = MSK_OUT0_TRIGGER_CAM0;
	app_regs.REG_OUT1_CONFIG = MSK_OUT1_TRIGGER_CAM1;
	
	app_regs.REG_RESERVED6 = 0;
	app_regs.REG_RESERVED7 = 0;
	
	//app_regs.REG_OUT_SET;
	//app_regs.REG_OUT_CLEAR;
	//app_regs.REG_OUT_TOGGLE;
	app_regs.REG_OUT_WRITE = 0;
	
	//app_regs.REG_IN_STATE;
}

void core_callback_registers_were_reinitialized(void)
{
	/* Update registers if needed */
	app_write_REG_START_AND_STOP(&app_regs.REG_START_AND_STOP);
	
	app_write_REG_CAM0_TRIGGER_CONFIG(&app_regs.REG_CAM0_TRIGGER_CONFIG);
	app_write_REG_CAM0_TRIGGER_INVERTED(&app_regs.REG_CAM0_TRIGGER_INVERTED);
	app_write_REG_CAM0_STROBE_SOURCE(&app_regs.REG_CAM0_STROBE_SOURCE);
	app_write_REG_CAM0_TRIGGER_FREQUENCY(&app_regs.REG_CAM0_TRIGGER_FREQUENCY);
	app_write_REG_CAM0_TRIGGER_DURATION_US(&app_regs.REG_CAM0_TRIGGER_DURATION_US);
	
	app_write_REG_CAM1_TRIGGER_CONFIG(&app_regs.REG_CAM1_TRIGGER_CONFIG);
	app_write_REG_CAM1_TRIGGER_INVERTED(&app_regs.REG_CAM1_TRIGGER_INVERTED);
	app_write_REG_CAM1_STROBE_SOURCE(&app_regs.REG_CAM1_STROBE_SOURCE);
	app_write_REG_CAM1_TRIGGER_FREQUENCY(&app_regs.REG_CAM1_TRIGGER_FREQUENCY);
	app_write_REG_CAM1_TRIGGER_DURATION_US(&app_regs.REG_CAM1_TRIGGER_DURATION_US);
	
	app_write_REG_OUT0_CONFIG(&app_regs.REG_OUT0_CONFIG);
	app_write_REG_OUT1_CONFIG(&app_regs.REG_OUT1_CONFIG);
	
	app_write_REG_OUT_WRITE(&app_regs.REG_OUT_WRITE);
	
	app_regs.REG_START_AND_STOP = B_START_CAM0 | B_START_CAM1;
	app_write_REG_START_AND_STOP(&app_regs.REG_START_AND_STOP);
}

/************************************************************************/
/* Callbacks: Visualization                                             */
/************************************************************************/
void core_callback_visualen_to_on(void)
{
	/* Update visual indicators */
	if (TCD0_CTRLA)
		set_LED_CAM0;
	else
		clr_LED_CAM0;
	
	if (TCC0_CTRLA)
		set_LED_CAM1;
	else
		clr_LED_CAM1;
}

void core_callback_visualen_to_off(void)
{
	/* Clear all the enabled indicators */
	clr_LED_CAM0;
	clr_LED_CAM1;
	clr_LED_PWR;
}

/************************************************************************/
/* Callbacks: Change on the operation mode                              */
/************************************************************************/
void core_callback_device_to_standby(void) {}
void core_callback_device_to_active(void) {}
void core_callback_device_to_enchanced_active(void) {}
void core_callback_device_to_speed(void) {}

/************************************************************************/
/* Callbacks: 1 ms timer                                                */
/************************************************************************/
uint16_t ms_counter = 0;

extern bool cam0_start_request;
extern bool cam1_start_request;
extern bool cam0_stop_request;
extern bool cam1_stop_request;
extern bool cam0_acquiring;
extern bool cam1_acquiring;

bool cam0_is_using_fixed_frequency = false;
bool cam1_is_using_fixed_frequency = false;

bool stop_cam0_when_possible = false;
bool stop_cam1_when_possible = false;

extern uint8_t cam0_freq_prescaler;
extern uint8_t cam1_freq_prescaler;
extern uint16_t cam0_freq_target_count;
extern uint16_t cam1_freq_target_count;
extern uint16_t cam0_freq_dutycyle;
extern uint16_t cam1_freq_dutycyle;

extern uint8_t cam0_pulse_prescaler;
extern uint8_t cam1_pulse_prescaler;
extern uint16_t cam0_pulse_target_count;
extern uint16_t cam1_pulse_target_count;



void core_callback_t_before_exec(void) {}
void core_callback_t_after_exec(void) {}
void core_callback_t_new_second(void)
{
	ms_counter = 0;
	
	if (cam0_start_request)
	{
		if (core_func_read_R_TIMESTAMP_SECOND() >= app_regs.REG_START_TIMESTAMP)
		{
			cam0_start_request = false;
			cam0_acquiring = true;
			
			if (core_bool_is_visual_enabled())
				set_LED_CAM0;
			
			if (app_regs.REG_CAM0_TRIGGER_CONFIG == MSK_TRG_SRC_INTERNAL)
			{
				timer_type0_pwm(&TCD0, cam0_freq_prescaler, cam0_freq_target_count, cam0_freq_dutycyle, INT_LEVEL_LOW, INT_LEVEL_LOW);
			}
			else
			{
				cam0_is_using_fixed_frequency = true;
			}
		}
	}

	if (cam0_stop_request)
	{
		if (core_func_read_R_TIMESTAMP_SECOND() >= app_regs.REG_STOP_TIMESTAMP)
		{
			if (cam0_is_using_fixed_frequency)
			{
				cam0_is_using_fixed_frequency = false;
				cam0_acquiring = false;
				clr_LED_CAM0;
			}
			else
			{
				stop_cam0_when_possible = true;
			}
		}
	}
	
	if (cam1_start_request)
	{
		if (core_func_read_R_TIMESTAMP_SECOND() >= app_regs.REG_START_TIMESTAMP)
		{
			cam1_start_request = false;
			cam1_acquiring = true;
			
			if (core_bool_is_visual_enabled())
				set_LED_CAM1;
			
			if (app_regs.REG_CAM1_TRIGGER_CONFIG == MSK_TRG_SRC_INTERNAL)
			{
				timer_type0_pwm(&TCC0, cam1_freq_prescaler, cam1_freq_target_count, cam1_freq_dutycyle, INT_LEVEL_LOW, INT_LEVEL_LOW);
			}
			else
			{
				cam1_is_using_fixed_frequency = true;
			}
		}
	}

	if (cam1_stop_request)
	{
		if (core_func_read_R_TIMESTAMP_SECOND() >= app_regs.REG_STOP_TIMESTAMP)
		{
			if (cam1_is_using_fixed_frequency)
			{
				cam1_is_using_fixed_frequency = false;
				cam1_acquiring = false;
				clr_LED_CAM1;
			}
			else
			{
				stop_cam1_when_possible = true;
			}
		}
	}
	
}
void core_callback_t_500us(void) {}
void core_callback_t_1ms(void)
{
	bool trigger_cam0 = false;
	bool trigger_cam1 = false;
	
	if (cam0_is_using_fixed_frequency)
	{			
		switch (app_regs.REG_CAM0_TRIGGER_CONFIG)
		{
			case MSK_TRG_SRC_INTERNAL_1Hz:   if (!(ms_counter % (1000/1)  )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_2Hz:   if (!(ms_counter % (1000/2)  )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_5Hz:   if (!(ms_counter % (1000/5)  )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_10Hz:  if (!(ms_counter % (1000/10) )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_20Hz:  if (!(ms_counter % (1000/20) )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_40Hz:  if (!(ms_counter % (1000/40) )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_50Hz:  if (!(ms_counter % (1000/50) )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_80Hz:  if (!(ms_counter % (1000/80) )) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_100Hz: if (!(ms_counter % (1000/100))) trigger_cam0 = true; break;
			case MSK_TRG_SRC_INTERNAL_125Hz: if (!(ms_counter % (1000/125))) trigger_cam0 = true; break;
		}
	}
	
	if (cam1_is_using_fixed_frequency)
	{
		switch (app_regs.REG_CAM1_TRIGGER_CONFIG)
		{
			case MSK_TRG_SRC_INTERNAL_1Hz:   if (!(ms_counter % (1000/1)  )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_2Hz:   if (!(ms_counter % (1000/2)  )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_5Hz:   if (!(ms_counter % (1000/5)  )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_10Hz:  if (!(ms_counter % (1000/10) )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_20Hz:  if (!(ms_counter % (1000/20) )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_40Hz:  if (!(ms_counter % (1000/40) )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_50Hz:  if (!(ms_counter % (1000/50) )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_80Hz:  if (!(ms_counter % (1000/80) )) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_100Hz: if (!(ms_counter % (1000/100))) trigger_cam1 = true; break;
			case MSK_TRG_SRC_INTERNAL_125Hz: if (!(ms_counter % (1000/125))) trigger_cam1 = true; break;
		}
	}
		
	if (trigger_cam0)
	{	
		set_io(PORTD, 0);
		core_func_mark_user_timestamp();
		timer_type0_enable (&TCD0, cam0_pulse_prescaler, cam0_pulse_target_count, INT_LEVEL_LOW);
	}
	
	if (trigger_cam1)
	{	
		set_io(PORTC, 0);
		core_func_mark_user_timestamp();
		timer_type0_enable (&TCC0, cam0_pulse_prescaler, cam0_pulse_target_count, INT_LEVEL_LOW);
	}
		
	
	if (trigger_cam0)
	{
		trigger_cam0 = false;
		
		if (app_regs.REG_CAM0_EVENT_CONFIG == MSK_EVENT_ON_TRIGGER)
		{
			app_regs.REG_CAM0_EVENT = B_TRIGGER;
			core_func_send_event(ADD_REG_CAM0_EVENT, false);
		}
	}
	
	if (trigger_cam1)
	{
		trigger_cam1 = false;
		
		if (app_regs.REG_CAM1_EVENT_CONFIG == MSK_EVENT_ON_TRIGGER)
		{
			app_regs.REG_CAM1_EVENT = B_TRIGGER;
			core_func_send_event(ADD_REG_CAM1_EVENT, false);
		}
	}	
	
	ms_counter++;
}

/************************************************************************/
/* Callbacks: uart control                                              */
/************************************************************************/
void core_callback_uart_rx_before_exec(void) {}
void core_callback_uart_rx_after_exec(void) {}
void core_callback_uart_tx_before_exec(void) {}
void core_callback_uart_tx_after_exec(void) {}
void core_callback_uart_cts_before_exec(void) {}
void core_callback_uart_cts_after_exec(void) {}

/************************************************************************/
/* Callbacks: Read app register                                         */
/************************************************************************/
bool core_read_app_register(uint8_t add, uint8_t type)
{
	/* Check if it will not access forbidden memory */
	if (add < APP_REGS_ADD_MIN || add > APP_REGS_ADD_MAX)
		return false;
	
	/* Check if type matches */
	if (app_regs_type[add-APP_REGS_ADD_MIN] != type)
		return false;
	
	/* Receive data */
	(*app_func_rd_pointer[add-APP_REGS_ADD_MIN])();	

	/* Return success */
	return true;
}

/************************************************************************/
/* Callbacks: Write app register                                        */
/************************************************************************/
bool core_write_app_register(uint8_t add, uint8_t type, uint8_t * content, uint16_t n_elements)
{
	/* Check if it will not access forbidden memory */
	if (add < APP_REGS_ADD_MIN || add > APP_REGS_ADD_MAX)
		return false;
	
	/* Check if type matches */
	if (app_regs_type[add-APP_REGS_ADD_MIN] != type)
		return false;

	/* Check if the number of elements matches */
	if (app_regs_n_elements[add-APP_REGS_ADD_MIN] != n_elements)
		return false;

	/* Process data and return false if write is not allowed or contains errors */
	return (*app_func_wr_pointer[add-APP_REGS_ADD_MIN])(content);
}