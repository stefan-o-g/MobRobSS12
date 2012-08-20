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
 * @file 	behaviour_follow_light.c
 * @brief 	Folge der Ldichquelle
 * @author 	IKARUS
 * @date 	26.04.2012
 */


#include "bot-logic/bot-logic.h"
#ifdef BEHAVIOUR_FOLLOW_LIGHT_AVAILABLE

#include "display.h"
#include "ui/available_screens.h"

static uint8_t state = 0;	/*!< Status des Follow-Light-Verhaltens */
static int16_t bot_speed = 35;
static uint16_t waypoint_counter = 0;

#define FOLLOW_LIGHT_INIT 0
#define FOLLOW_LIGHT_RUN 1
#define FOLLOW_LIGHT_END 2

#define LDR_CORRECT 45
#define TOLERANCE 15
#define MAX_WAYPOINTS 8
#define DRIVE_SPEED 55

/*!
 * Verhaltenslogik.
 *
 * @param *data der Verhaltensdatensatz
 */

void bot_follow_light_behaviour(Behaviour_t * data) {
	switch(state){
	case FOLLOW_LIGHT_INIT:
#ifdef DISPLAY_FOLLOW_LIGHT_AVAILABLE
			display_screen = 11; // Ausgabescreen Nr. 11 anzeigen.
								 // Das ist der Bildschirm mit den ausgaben diese App.
			display_clear();
			display_printf("Follow Light B. v0.1");
			state = FOLLOW_LIGHT_RUN;

#endif
		break;
	case FOLLOW_LIGHT_RUN:
#ifdef DISPLAY_FOLLOW_LIGHT_AVAILABLE
				display_cursor(2, 1);
				display_printf("LDRs: L=%04d R=%04d", sensLDRL, sensLDRR);
				display_cursor(3, 1);
#endif
			if ((sensLDRL - sensLDRR) -LDR_CORRECT > TOLERANCE){
				// Links dunkler (nach Rechts drehen)
#ifdef DISPLAY_FOLLOW_LIGHT_AVAILABLE
				display_printf("Rechts drehen");
#endif
				speedWishLeft = 35;
				speedWishRight = -35;
			} else if ((sensLDRL - sensLDRR) -LDR_CORRECT < -TOLERANCE){
				// Rechts dunkler (nach Links drehen)
#ifdef DISPLAY_FOLLOW_LIGHT_AVAILABLE
				display_printf("Links drehen");
#endif
				speedWishLeft = -35;
				speedWishRight = 35;
			} else {
				// Grade aus
				waypoint_counter++;
#ifdef DISPLAY_FOLLOW_LIGHT_AVAILABLE
				display_printf("Grade fahren");
				display_cursor(4, 1);
				display_printf("WayP.=%d, Speed=%d", waypoint_counter, bot_speed);
#endif
				bot_drive_distance(data, 0, bot_speed, 7);
			}
		break;
	default:
		return_from_behaviour(data);
		break;
	}
}

void bot_run_from_light_behaviour(Behaviour_t * data) {
	bot_speed = -DRIVE_SPEED;
	bot_follow_light_behaviour(data);
}

void bot_follow_light_stack_behaviour(Behaviour_t * data) {
	// INFO: Für dieses verhalten muss
	// BEHAVIOUR_DRIVE_STACK_AVAILABLE aktiviert (available_behaviours.h)
	// und aktiv (bot-logic.c) sein.
	if (waypoint_counter >= MAX_WAYPOINTS) {
#ifdef DISPLAY_FOLLOW_LIGHT_AVAILABLE
		display_cursor(3, 1);
		display_printf("Drive Stack");
		display_cursor(4, 1);
		display_printf("");
#endif
		bot_drive_stack(data);
		waypoint_counter = 0;
	} else {
		bot_follow_light_behaviour(data);
	}
}

/*!
 * Rufe das Follow-Light-Verhalten auf
 * @param *caller	Der obligatorische Verhaltensdatensatz des Aufrufers
 */
void bot_follow_light(Behaviour_t * caller) {
	state = FOLLOW_LIGHT_INIT;
	bot_speed = DRIVE_SPEED;
	switch_to_behaviour(caller, bot_follow_light_behaviour, BEHAVIOUR_OVERRIDE);
}

/*!
 * Rufe das Follow-Light-Verhalten auf
 * @param *caller	Der obligatorische Verhaltensdatensatz des Aufrufers
 */
void bot_run_from_light(Behaviour_t * caller) {
	state = FOLLOW_LIGHT_INIT;
	bot_speed = -DRIVE_SPEED;
	switch_to_behaviour(caller, bot_run_from_light_behaviour, BEHAVIOUR_OVERRIDE);
}

void bot_follow_light_stack(Behaviour_t * caller) {
	state = FOLLOW_LIGHT_INIT;
	bot_speed = DRIVE_SPEED;
	waypoint_counter = 0;
	switch_to_behaviour(caller,  bot_follow_light_stack_behaviour, BEHAVIOUR_OVERRIDE);
}

#endif // BEHAVIOUR_FOLLOW_LIGHT_AVAILABLE
