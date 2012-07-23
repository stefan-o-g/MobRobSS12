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

int display_remote(){
	bot_sens_isr();
	int16 c = ir_read();
	int16 n = c & ~(1<<11);


	if(c != 0){
		display_clear();
		display_cursor(1,1);
		display_printf("Code: %d N:%d",c,n);	
		return n;
	}
	return 0;
}

enum{
	stand,
	follow_light,
	avoid_light,
	follow_line
};

int state = stand;

void dostuff(){
	
	int code = display_remote();
	switch(code){
	case 12289: state = stand; //1 tv
	break;
	case 12290: state = follow_light; //2 tv
	break;
	case 12291: state = avoid_light; //3 tv
	break;
	case 12292:  state = follow_line; //4 tv
	break;
	case 12293: //5 tv
	break;
	case 12294: //6 tv
	break;
	case 12295: //7 tv
	break;
	case 12296: //8 tv
	break;
	case 12297:  //9 tv
	break;
	}
	
	switch(state){
		case follow_light: light(1);
		break;		
		case avoid_light: light(-1);
		break;
		case stand: motor_set(0,0);
		break; 
		case follow_line: line();
		break;
	}
}
