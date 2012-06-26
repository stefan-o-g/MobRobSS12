
void light(int dir){
	static int16 lastpos = 0;
	static int16 maxL = 0;
	static int16 maxR = 0;
	maxL = (sensLDRL > maxL)?sensLDRL:maxL;
	maxR = (sensLDRR > maxR)?sensLDRR:maxR;


	display_clear();
	int16 lightpos = sensLDRR - sensLDRL;
	display_cursor(2,1);
	display_printf("R:%d L: %d",sensLDRR,sensLDRL);
	display_cursor(3,1);
	display_printf("Lightpos: %d",lightpos);
	
	display_cursor(4,1);
	display_printf("Sum: %d",sensLDRL+sensLDRR);
	

	int16 speed = 5;
	int16 trsh = 50;
	if(lightpos > trsh){
		motor_set(-speed,speed);
	}else if(lightpos < -trsh){
		motor_set(speed,-speed);
	}else{
		if(abs(lightpos) < 30){
			if(sensLDRL+sensLDRR < 800){
				motor_set(speed*dir,speed*dir);
			}else{
				motor_set(0,0);
			}
		}
	}
	


	lastpos = lightpos;
}

void line(){
	int r_correction = 0;
	int l_correction = 0;
	sensLineR += r_correction;
	sensLineL+= l_correction;
	
	display_clear();
	display_cursor(1,1);
	display_printf("mouseSensL: %d", sensLineL);
	display_cursor(2,1);
	display_printf("mouseSensR: %d", sensLineR);
	
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
