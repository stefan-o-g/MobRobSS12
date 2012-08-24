#include "scanner.h"
#include "andy.h"

struct token token;
char buf[50];

struct token* get_token(void){
	return &token;
}

struct token* next_token(void){
	static int count = 0;
	command_t* cmd = read_command();
	while(cmd == (void*)0){
		cmd = read_command();
	}
	if(cmd->data_l == cmd->data_r){

		buf[cmd->payload] = '\0';
		token.type = cmd->data_l;





		display_clear();
		display_cursor(1,1);
		display_printf("%d Token: %d",++count, token.type);
		display_cursor(2,1);
		display_printf("Value: %s", read_payload(cmd, (uint8_t*)buf));
		token.text = buf;


	}else{
		display_clear();
		display_cursor(1,1);
		display_printf("token panic !!");
		delay(10000);
	}
	return &token;
}
