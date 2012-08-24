#ifndef MR2012_H_
#define MR2012_H_

#include "sensor.h"
#include "motor.h"
#include "display.h"
#include "ir-rc5.h"
#include "command.h"
#include "uart.h"

/**
 * 	Zustände (Programme/Funktionen)
 */
enum funktion{
	stand,
	motte1,
	karkerlake1,
	motte2,
	karkerlake2,
	acht,
	linie,
	move
};

uint8_t* read_payload(command_t* cmd, uint8_t* buffer);
command_t* read_command(void);

/**
 * Einstigspunkt für das Programm.
 * Diese Funktion sollte in der Hauptschleife ausgeführt werden.
 */
void entry_point(void);

/**
 * Lichtgesteuerte verhalten wie Motte und Kakerlake 
 * @param dir gibt an in welche rechtung gefahren werden soll wenn licht gleichmäßig einstrahlt 1 = zum Licht hin 0 = vom Licht weg.
 * 
 */
void light(int dir);

/**
 * 	Linie folgen.
 */
void line(void);

/**
 * 	Hilfsfunktion zum lesen des Fernbedinungscodes.
 * 	Passt den erhaltenen Code an indem das Alternierende 11 Bit immer auf 0 gesetzt;
 */
int get_code(void);


/**
 * Lässt den Bot einer Lichtquelle folgen bzw. auf sie zufahren.
 * Die Funktion muss dafür öfter bzw. in der Dauerschleife aufgerufen werden.
 * Aufgabe 1 a)
 */
void motte_nonbehav(void);

/**
 * Lässt den Bot vor einer Lichtquelle flüchten bzw. ihn von ihr wegfahren.
 * Die Funktion muss dafür öfter bzw. in der Dauerschleife aufgerufen werden.
 * Aufgabe 1 a)
 */
void kakerlake_nonbehav(void);

/**
 * Fährt eine "8" mit 40x80cm.
 * Aufabe 1 c)
 */
void acht_nonbehav(void);


/**
 * Vergleicht ob zwei Werte "fast" gleich sind.
 * @return 1 für fast gleich, 0 für nicht fast gleich
 * @param v1 Wert 1
 * @param v2 Wert 2
 * @param delta Der Wert um den sich v1 maximal +/- von v2 unterscheiden darf.
 */
int nearly_equal(int v1, int v2, int delta);
#endif
