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
 * @brief 	Folge der dichquelle
 * @author 	IKARUS
 * @date 	26.04.2012
 */

#ifndef BEHAVIOUR_FOLLOW_LIGHT_H_
#define BEHAVIOUR_FOLLOW_LIGHT_H_

#ifdef BEHAVIOUR_FOLLOW_LIGHT_AVAILABLE

/*!
 * Verhaltenslogik.
 *
 * @param *data der Verhaltensdatensatz
 */
void bot_follow_light_behaviour(Behaviour_t * data);
void bot_run_from_light_behaviour(Behaviour_t * data);
void bot_follow_light_stack_behaviour(Behaviour_t * data);
/*!
 * Rufe das Follow-Light-Verhalten auf
 * @param *caller	Der obligatorische Verhaltensdatensatz des Aufrufers
 */
void bot_follow_light(Behaviour_t * caller);
void bot_run_from_light(Behaviour_t * caller);
void bot_follow_light_stack(Behaviour_t * caller);

#endif // BEHAVIOUR_FOLLOW_LIGHT_AVAILABLE
#endif // BEHAVIOUR_FOLLOW_LIGHT_H_
