/*
 * andy.c
 *
 *  Created on: 08.05.2012
 *      Author: andy
 */

#include "sensor.h"
#include "motor.h"
#include "display.h"
#include "ir-rc5.h"
#include "andy.h"

void motte_nonbehav(){
	//Sensorwerte aktualliseren und Bildschirmausgabe
	sensor_update();
	display_clear();
	display_printf("Motte");
	display_cursor(2,1);
	display_printf("R = %d", sensLDRR);
	display_cursor(3,1);
	display_printf("L = %d", sensLDRL);

	//Dient als Regulierung der Motoransteuerung
	//im Grenzbereich zwischen den Sensorwerten
	int stopMotor;

	//Links drehen solange der linke Sensor kleiner/heller
	stopMotor=0;
	while(sensLDRR > sensLDRL && stopMotor++ < 20){
		//Schnell drehen wenn der Unterschied groß ist...
		if(sensLDRR > sensLDRL*2)
			motor_set(-BOT_SPEED_NORMAL, BOT_SPEED_NORMAL);
		//...sonst langsam drehen
		else
			motor_set(-BOT_SPEED_FOLLOW, BOT_SPEED_MEDIUM);
		display_cursor(4,1);
		display_printf("Turning left...");
		sensor_update();
	}
	//Rechts drehen solange der rechte Sensor kleiner/heller
	stopMotor=0;
	while(sensLDRR < sensLDRL && stopMotor++ < 20){
		//Schnell drehen wenn der Unterschied groß ist...
		if(sensLDRR*2 < sensLDRL)
			motor_set(BOT_SPEED_NORMAL, -BOT_SPEED_NORMAL);
		//...sonst langsam drehen
		else
			motor_set(BOT_SPEED_MEDIUM, -BOT_SPEED_FOLLOW);
		display_cursor(4,1);
		display_printf("Turning right...");
		sensor_update();
	}

	//Richtung einige Zeit beibehalten wenn Sensorwerte gleich waren
	stopMotor=0;
	while(nearly_equal(sensLDRR, sensLDRL, 2) && stopMotor++ < 120){
		motor_set(BOT_SPEED_MEDIUM, BOT_SPEED_MEDIUM);
		display_cursor(4,1);
		display_printf("Following...");
		sensor_update();
	}

	//stopMotor=0;
	//display_cursor(4,1);
	//display_printf("Stop");
	//motor_set(BOT_SPEED_STOP,BOT_SPEED_STOP);
}


void kakerlake_nonbehav(){
	//Sensorwerte aktualliseren und Bildschirmausgabe
	sensor_update();
	display_clear();
	display_printf("Kakerlake");
	display_cursor(2,1);
	display_printf("R = %d", sensLDRR);
	display_cursor(3,1);
	display_printf("L = %d", sensLDRL);

	//Dient als Regulierung der Motoransteuerung
	//im Grenzbereich zwischen den Sensorwerten
	int stopMotor;

	//Links drehen solange der linke Sensor größer/dunkler
	if(sensLDRR < sensLDRL){
		stopMotor=0;
		while(sensLDRR < sensLDRL && stopMotor++ < 20){
			//Schnell drehen wenn der Unterschied groß ist...
			if(sensLDRR < sensLDRL*2)
				motor_set(-BOT_SPEED_NORMAL, BOT_SPEED_NORMAL);
			//...sonst langsam drehen
			else
				motor_set(-BOT_SPEED_FOLLOW, BOT_SPEED_MEDIUM);
			display_cursor(4,1);
			display_printf("Turning left...");
			sensor_update();
		}
	}
	//Rechts drehen solange der rechte Sensor größer/dunkler
	else{
		stopMotor=0;
		while(sensLDRR > sensLDRL && stopMotor++ < 20){
			//Schnell drehen wenn der Unterschied groß ist...
			if(sensLDRR*2 > sensLDRL)
				motor_set(BOT_SPEED_NORMAL, -BOT_SPEED_NORMAL);
			//...sonst langsam drehen
			else
				motor_set(BOT_SPEED_MEDIUM, -BOT_SPEED_FOLLOW);
			display_cursor(4,1);
			display_printf("Turning right...");
			sensor_update();
		}
	}

	//Richtung einige Zeit beibehalten wenn Sensorwerte ca. gleich waren
	stopMotor=0;
	int ldrr_old=0, ldrl_old=0;
	while(nearly_equal(sensLDRR, sensLDRL, 5) && stopMotor++ < 120){
		ldrr_old = sensLDRR;
		ldrl_old = sensLDRL;
		sensor_update();
		display_cursor(4,1);
		if(ldrr_old > sensLDRR && ldrl_old > sensLDRL)
			motor_set(-BOT_SPEED_MEDIUM, -BOT_SPEED_MEDIUM);
		else
			motor_set(BOT_SPEED_MEDIUM, BOT_SPEED_MEDIUM);
		display_printf("Getting away...");
	}

	//stopMotor=0;
	//display_cursor(4,1);
	//display_printf("Stop");
	//motor_set(BOT_SPEED_STOP,BOT_SPEED_STOP);
}


void acht_nonbehav(){
	static int distr = 18;
	static int distl = 18;
	int ir = ir_read();
	int i,j;

	display_clear();
	display_printf("Acht!");
	display_cursor(2,1);
	display_printf("DistR=%d  IR=%5d", distr, ir);
	display_cursor(3,1);
	display_printf("DistL=%d  IR=%5d", distl, ir);

	display_cursor(4,1);
	display_printf("Rechtskurve");
	for(i=0; i<distr; i++)
		for(j=0; j<distr; j++){
			motor_set(BOT_SPEED_FAST, 0); //BOT_SPEED_SLOW

			ir = ir_read();
			switch(ir){
				//hoch 8208 oder 10256 = pfeil hoch
				case 8208:
				case 10256:
					distr++;
					break;
				//runter 8209 oder 10257 = pfeil runter
				case 8209:
				case 10257:
					distr --;
					break;
				//hoch 8214 oder 10262 = pfeil rechts
				case 8214:
				case 10262:
					distl++;
					break;
				//runter 8213 oder 10261 = pfeil links
				case 8213:
				case 10261:
					distl--;
					break;
				default:
					break;
			}

			display_cursor(2,1);
			display_printf("DistR=%d", distr);
			ir = 0;
		}

	display_cursor(4,1);
	display_printf("Linkskurve ");
	for(i=0; i<distl; i++)
		for(j=0; j<distl; j++){
			motor_set(0, BOT_SPEED_FAST); //BOT_SPEED_SLOW

			ir = ir_read();
			switch(ir){
				//hoch 8208 oder 10256 = pfeil hoch
				case 8208:
				case 10256:
					distr++;
					break;
				//runter 8209 oder 10257 = pfeil runter
				case 8209:
				case 10257:
					distr --;
					break;
				//hoch 8214 oder 10262 = pfeil rechts
				case 8214:
				case 10262:
					distl++;
					break;
				//runter 8213 oder 10261 = pfeil links
				case 8213:
				case 10261:
					distl--;
					break;
				default:
					break;
			}

			display_cursor(3,1);
			display_printf("DistL=%d", distl);
			ir = 0;
		}
}

int nearly_equal(int v1, int v2, int delta){
	int diff = v1 - v2;
	if(diff < 0)
		diff = -diff;
	if(diff > delta)
		return 0;
	return 1;
}


//folge dem licht bzw vom licht abhauen jenachdem wie dir gesetzt ist (1 / -1)
void light(int dir){
	display_clear();

	//gibt die lichtposition an
	int16 lightpos = sensLDRR - sensLDRL;
	
	//debug ausgabe
	display_cursor(2,1);
	display_printf("R:%d L: %d",sensLDRR,sensLDRL);
	display_cursor(3,1);
	display_printf("Lightpos: %d",lightpos);
	
	display_cursor(4,1);
	display_printf("Sum: %d",sensLDRL+sensLDRR);
	
	//speed variable
	int16 speed = 5;

	//grenzwert der entscheidet wann wieder zum licht hingedreht wird
	int16 trsh = 50;
	
	//wenn licht einseitig einstrahlt zum licht hindrehen
	if(lightpos > trsh){
		motor_set(-speed,speed);
	}else if(lightpos < -trsh){
		motor_set(speed,-speed);
	
	//beide sensoren sind mehr oder weniger gleich
	}else{
			//beide sensoren werden hell betrahlt = fahren
			if(sensLDRL+sensLDRR < 800){
				motor_set(speed*dir,speed*dir);
			//beide sensoren sind dunkel = stehenbleiben
			}else{
				motor_set(0,0);
			}
	}
}

void line(){
	// korrections werte um sensorfehler auszugleichen
	int r_correction = 0;
	int l_correction = 0;
	sensLineR += r_correction;
	sensLineL+= l_correction;
	
	//debug ausgabe
	display_clear();
	display_cursor(1,1);
	display_printf("mouseSensL: %d", sensLineL);
	display_cursor(2,1);
	display_printf("mouseSensR: %d", sensLineR);
	
	//delta gibt auskunft zur unterschied der beiden sensoren
	int delta = sensLineL - sensLineR;
	display_cursor(3,1);
	display_printf("delta: %d", delta);


	if(delta < -300){
		motor_set(4,-4);	
	}else if(delta > 300){
		motor_set(-4,4);
	}else{
		if((sensLineL + sensLineR) < 100 ){
			motor_set(1,5);
		}else{
			motor_set(2,2);
		}
	}	
}

int get_code(){
	//Fernbedinungscode Lesen
	int16 c = ir_read();
	//störendes Bit auf 0 setzen damit der Code für die Tasten gleich bleibt
	int16 n = c & ~(1<<11);
}


	motte2,
	karkerlake2,
	acht,


void entry_point(){
	//Fernbedinungscode abfragen und zustand wechseln
	int code = get_code();
	switch(code){
	case 12289: state = stand; //1 tv
	break;
	case 12290: state = motte1; //2 tv
	break;
	case 12291: state = karkerlake1; //3 tv
	break;
	case 12292:  state = motte2; //4 tv
	break;
	case 12293: state = karkerlake2; //5 tv
	break;
	case 12294: state = acht; //6 tv
	break;
	case 12295: state = linie; //7 tv
	break;
	case 12296: //8 tv
	break;
	case 12297:  //9 tv
	break;
	}
	
	switch(state){
		case motte1: light(1);
		break;		
		case karkerlake1: light(-1);
		break;
		case stand: motor_set(0,0);
		break; 
		case linie: line();
		break;
		case motte2: motte_nonbehav();
		break;
		case karkerlake2: kakerlake_nonbehav();;
		break;
		case acht: acht_nonbehav();
		break;
	}
}
