#ifndef __FWCFG_H__
#define __FWCFG_H__

struct firmware_config {
	/*
	 * Button GPIO number, must be PL bank,
	 * only the number after PL is needed.
	 * e.g. PL3 is 3
	 * Many boards use PL3 here.
	 * Set -1 here to disable this functionality.
	 */
	int button_gpio_pl;
	/*
	 * LED GPIO number, same rule as the button GPIO.
	 * Many boards use PL10 here.
	 * Set -1 here to disable this functionality.
	 */
	int led_gpio_pl;
	/*
	 * Button press time to trigger reboot, unit is second.
	 * The default value is currently 3s.
	 */
	int button_press_time;
	/*
	 * Button pressed status, the voltage status when the button
	 * is pressed.
	 * Many board is 0 here, means that the voltage is 0 when
	 * the button is pressed.
	 */
	int button_pressed_voltage;

	int reserved[0x3d];
};

extern struct firmware_config fwcfg;

#endif
