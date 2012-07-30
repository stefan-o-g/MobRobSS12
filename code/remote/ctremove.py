#!/usr/bin/python
'''ctremote.py  - Skript um den ctBot per WLAN fernzusteuern...'''

from sys import argv, exit
from os import system
import socket
import thread

###############################################################
#CODE TAKEN FROM: http://code.activestate.com/recipes/134892/
class _Getch:
    """Gets a single character from standard input.  Does not echo to the
screen."""
    def __init__(self):
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()


class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        return msvcrt.getch()

#CODE TAKEN FROM: http://code.activestate.com/recipes/134892/
###############################################################

rec_data = ""		#buffer for data received from the bot
rec_size = 1024		#max size of buffer (rec_data)

udp_sock = 0 				#socket for sending data
udp_bot_ip = "192.168.0.9"	#ip of the bot
udp_port = 10002			#port



def help():
	print "COMMANDS:"
	print "subcmd <subcommand>"
	print "   Send a subcommand to the Bot."
	print "move"
	print "   Control the bot using the arrow-keys."
	print "help"
	print "   Print this."
	print "exit"
	print "   Exit this script."
	

##################################################################

def openSocket():
	global udp_sock, udp_ip, udp_port

	print "#openSocket(): Using Bot-IP ", udp_bot_ip
	print "#openSocket(): Using port ", udp_port 

	udp_sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
	udp_sock.sendto( "Hello ctBot!", (udp_bot_ip, udp_port) )

##################################################################

def send_cmd(subcmd, ldata="\x03\x04", rdata="\x05\x06", payload="\x00", data=""):
	#start (>) + 1byte command  + 1byte subcommand + 1byte payload 
	#+ 2byte left data + 2byte right data + 2byte seq nr + end (<) + data
	cmd = ">#" + subcmd + payload + ldata + rdata + "\x07\x08<"	+ data

	global udp_sock
	udp_sock.sendto( cmd, (udp_bot_ip, udp_port) )

##################################################################

#recv will run as a thread, setting the variable "rec" when data was received
def recv():
	global rec, rec_data, rec_size

	sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
	sock.bind( ("",10002) )

	while True:
		rec_data = sock.recv(rec_size)
		if rec_data != "":
			print "\n#recv(): Data received..."
			#TODO: do something with rec_data here
			rec_data = ""

##################################################################

def user_input_eval(usrin):
	cmd_list = usrin.split()

	try:
		if cmd_list[0] == "subcmd":
			if cmd_list[1] == "1":
				send_cmd('\x01')
			elif cmd_list[1] == "2":
				send_cmd('\x02')
			elif cmd_list[1] == "3":
				send_cmd('\x03')
			elif cmd_list[1] == "4":
				send_cmd('\x04')
			elif cmd_list[1] == "5":
				send_cmd('\x05')
			elif cmd_list[1] == "6":
				send_cmd('\x06')
			elif cmd_list[1] == "7":
				send_cmd('\x07')
			elif cmd_list[1] == "8":
				send_cmd('\x08')
			elif cmd_list[1] == "9":
				send_cmd('\x09')
		elif cmd_list[0] == "move":
			move()
		elif cmd_list[0] == "exit":
			send_cmd('\x01')
			global run
			run = False
		elif cmd_list[0] == "help":
			help()
		else:
			print "\n#user_input_eval(): Unknown command... Try 'help'..."
	except:
		print "\n#user_input_eval(): Command ERROR! Try 'help'..."
		


##################################################################

def move():
	getch = _Getch()
	foo = getch()
	while foo != 'q':
		if foo == 'w':
			send_cmd('m',"\xff\x00","\xff\x00")
		elif foo == 's':
			send_cmd('m',"\x10\x80","\x10\x80")
		elif foo == 'a':
			send_cmd('m',"\x10\x80","\x10\x00")
		elif foo == 'd':
			send_cmd('m',"\x10\x00","\x10\x80")
		elif foo == 'e':
			send_cmd('m',"\x00\x00","\x00\x00")
		foo = getch()
	send_cmd('m',"\x00\x00","\x00\x00")
		

##################################################################

#"main":

openSocket()
thread.start_new_thread(recv,())

run=True
while run:
	try:
		user_input_eval(raw_input("ctBot-remote $ "))
		#udp_sock.close()

	except KeyboardInterrupt:
		print "\n#Woah, STRG+C! Stopping Bot and exit..."
		send_cmd('\x01')
		#udp_sock.close()
		exit()

udp_sock.close()







