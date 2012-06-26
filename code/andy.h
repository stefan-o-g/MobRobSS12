
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
