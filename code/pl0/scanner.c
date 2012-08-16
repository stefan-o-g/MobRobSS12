#include "scanner.h"
#include "command.h"
#include "andy.h"

uint8_t buf[50];
struct token token;

struct token* get_token(void){
	return &token;
}

struct token* next_token(void){
		command_t* cmd = read_command();
		if(cmd->data_l != cmd->data_r){
			token.type = T_UNKNOWN;
			return &token;
		}


		read_payload(cmd, buf);
		buf[cmd->payload] = '\0';
		token.text = (char*)buf;
		token.type = cmd->data_l;


		return &token;
}
