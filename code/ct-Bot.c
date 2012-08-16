/*
 * c't-Bot
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 */

/*!
 * @file 	ct-Bot.c
 * @brief 	Bot-Hauptprogramm
 * @author 	Benjamin Benz (bbe@heise.de)
 * @date 	26.12.05
 */

#include "ct-Bot.h"

#ifdef MCU
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/wdt.h>
	#include "bot-2-pc.h"
	#include <avr/eeprom.h>
	#include "i2c.h"
	#include "sp03.h"
#endif
//#include <avr/delay.h> //Andy
#ifdef PC
	#include "bot-2-sim.h"
	#include "tcp.h"
	#include "eeprom-emu.h"
	#include <stdio.h>
	#include <time.h>
	#include <sys/time.h>
#endif

#ifdef TWI_AVAILABLE
	#include "twi.h"
#endif

#include "global.h"
#include "display.h"
#include "led.h"
#include "ena.h"
#include "shift.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "sensor.h"
#include "log.h"
#include "motor.h"
#include "sensor-low.h"
#include "bot-logic/bot-logik.h"
#include "mouse.h"

#include "command.h"
#include "ir-rc5.h"
#include "rc5.h"
#include "timer.h"
#include "mmc.h"
#include "mmc-emu.h"
#include "mmc-vm.h"
#include "gui.h"
#include "ui/available_screens.h"
#include "os_thread.h"
#include "map.h"
#include "cmd_tools.h"
#include "wlan.h"

#include "include/andy.h"




extern int send_mode; /* fix M.Geller 12.06.09 */

// WLAN: Zu übertragende Daten Definieren
#define WLAN_DATA_RECV 15
#define WLAN_DATA_FREQ 100




/**
 * kleiner ErrorHandler, welcher 3 Zeilen ausgeben kann
 * und dann in eine Endlosschleife geht
 * (MGeller 2009-06)
 * --------------------------------------------------
 *
 */
void errorHandler(char *msg1, char *msg2, char *msg3){
	display_clear();
	display_cursor(1,1);
	display_printf("ERROR:");
	display_cursor(2,1); display_printf("%s", msg1);
	display_cursor(2,2); display_printf("%s", msg2);
	display_cursor(2,3); display_printf("%s", msg3);
	while(1);
}




/*!
 * Der Mikrocontroller und der PC-Simulator brauchen ein paar Einstellungen,
 * bevor wir loslegen koennen.
 */
void init(void) {
	#ifdef MCU
		PORTA=0; DDRA=0;	// Alles Eingang -> alles Null
		PORTB=0; DDRB=0;
		PORTC=0; DDRC=0;
		PORTD=0; DDRD=0;

		wdt_disable();	// Watchdog aus!
		#ifdef OS_AVAILABLE
			os_create_thread((uint8_t *)SP, NULL);	// Hauptthread anlegen
		#endif
		timer_2_init();

		/* Ist das ein Power on Reset? */
		#ifdef __AVR_ATmega644__
			if ((MCUSR & 1) == 1) {
				MCUSR &= ~1;	// Bit loeschen
		#else
			if ((MCUCSR & 1) == 1) {
				MCUCSR &= ~1;	// Bit loeschen
		#endif
			delay(100);
			asm volatile("jmp 0");
		}

		delay(100);
		#ifdef RESET_INFO_DISPLAY_AVAILABLE
			#ifdef __AVR_ATmega644__
				reset_flag = MCUSR & 0x1F;	//Lese Grund fuer Reset und sichere Wert
				MCUSR = 0;	//setze Register auf 0x00 (loeschen)
			#else
				reset_flag = MCUCSR & 0x1F;	//Lese Grund fuer Reset und sichere Wert
				MCUCSR = 0;	//setze Register auf 0x00 (loeschen)
			#endif
			uint8 resets = eeprom_read_byte(&resetsEEPROM) + 1;
			eeprom_write_byte(&resetsEEPROM, resets);
		#endif	// RESET_INFO_DISPLAY_AVAILABLE
	#endif	// MCU

	#ifdef UART_AVAILABLE
		uart_init();
	#endif
	#ifdef BOT_2_PC_AVAILABLE
		bot_2_pc_init();
	#endif
	#ifdef PC
		if (init_eeprom_man(0) != 0) {
			LOG_ERROR("EEPROM-Manager nicht korrekt initialisiert!");
		}
	#endif
	#ifdef DISPLAY_AVAILABLE
		display_init();
	#endif
	#ifdef LED_AVAILABLE
		LED_init();
	#endif
	motor_init();
	bot_sens_init();
	#ifdef BEHAVIOUR_AVAILABLE
		bot_behave_init();
	#endif
	#ifdef MCU
		#ifdef RC5_AVAILABLE
			ir_init();
		#endif
	#endif
	#ifdef MMC_AVAILABLE
		mmc_init();
	#endif
	#ifdef MAUS_AVAILABLE
		maus_sens_init();
	#endif
	#ifdef MAP_AVAILABLE
		map_init();
	#endif
	#ifdef LOG_MMC_AVAILABLE
		log_mmc_init();
	#endif
	#ifdef I2C_AVAILABLE
		i2c_init(42);	// 160 kHz
	#endif
	#ifdef TWI_AVAILABLE
		Init_TWI();
	#endif
	#ifdef DISPLAY_AVAILABLE
		gui_init();
	#endif
}

#ifdef MCU
/*!
 * Hauptprogramm des Bots. Diese Schleife kuemmert sich um seine Steuerung.
 */
int main(void) {
#endif	// MCU

#ifdef PC
/*!
 * Hauptprogramm des Bots. Diese Schleife kuemmert sich um seine Steuerung.
 */
int main(int argc, char * argv[]) {
	/* Kommandozeilen-Argumente auswerten */
	hand_cmd_args(argc, argv);

	printf("c't-Bot\n");

	/* Bot2Sim-Kommunikation initialisieren */
	bot_2_sim_init();
	receive_until_Frame(CMD_DONE);
	command_write(CMD_DONE, SUB_CMD_NORM ,(int16*)&simultime,0,0);
	flushSendBuffer();
#endif	// PC

	/* Alles initialisieren */
	init();

	/*! Hauptschleife des Bots */
	for(;;) {
		#ifdef MCU
			bot_sens_isr();
		#endif

		/*ANFANG: ANDY*/
		//motte_nonbehav();
		//kakerlake_nonbehav();
		/*uint8_t buffer[100];

		command_t* cmd = read_command();
		if(cmd != ((void*)0)){
			read_payload(cmd,buffer);
			display_cursor(1,1);
			display_printf("P: %d",cmd->payload);
			display_cursor(2,1);
			display_printf("l: %d r: %d",cmd->data_l,cmd->data_r);
			display_cursor(3,1);
			display_printf("D: %s", buffer);
			//motor_set(cmd->data_l, cmd->data_r);
		}
		delay(10);
		*/

		display_clear();
		display_cursor(1,1);
		display_printf("init!");

		//entry_point();






		/*
		command_t* cmd = read_command();
		if(cmd->request.command == '#'){
			if(cmd->request.subcommand == 'm'){
				motor_set(cmd->data_l, cmd->data_r);
			}
		}
		*/

		//acht_nonbehav();


//			int ir = ir_read();
//			display_clear();
//			display_printf("IR TEST!");
//			display_cursor(2,1);
//			display_printf("IR = %d", ir);
//
//			int i;
//			for(i=0; i<250; i++)
//				for(i=0; i<250; i++)
//					for(i=0; i<250; i++){
//						display_cursor(3,1);
//						display_printf("i = %d", i);
//					}


		/*ENDE: ANDY*/

	}

	/* Falls wir das je erreichen sollten ;-) */
	return 1;
}


