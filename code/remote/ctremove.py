#!/usr/bin/python
'''ctremote.py  - Skript um den ctBot fernzusteuern...'''

from sys import argv, exit
from os import system
import socket
import struct
from thread import *
import time

rec = 0 #0 if no data received, 1 else

udp_sock = 0
udp_bot_ip = "192.168.0.9"
udp_port = 10002



def help():
	print "TODO: Put some usage-foo here..."

##################################################################

def openSocket():
	global udp_sock, udp_ip, udp_port

	print "#openSocket(): Using Bot-IP ", udp_bot_ip
	print "#openSocket(): Using port ", udp_port 

	udp_sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
	udp_sock.sendto( "Hello ctBot!", (udp_bot_ip, udp_port) )

##################################################################

def send_cmd(subcmd):
	#TODO: mehr parameter?!
	cmd = ">#" + subcmd + "\x02\x03\x04\x05\x06\x07\x08<"

	global udp_sock
	udp_sock.sendto( cmd, (udp_bot_ip, udp_port) )

##################################################################

#recv will run as a thread, setting the variable "rec" when data was received
def recv():
	global rec

	sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
	sock.bind( ("",10002) )

	while True:
		print "#recv(): " + sock.recv(1024) + "\n"
		rec = 1

##################################################################

def user_input_eval(usrin):
	if usrin == "1":
		send_cmd('\x01')
	elif usrin == "2":
		send_cmd('\x02')
	elif usrin == "3":
		send_cmd('\x03')
	elif usrin == "4":
		send_cmd('\x04')
	elif usrin == "5":
		send_cmd('\x05')
	elif usrin == "6":
		send_cmd('\x06')
	elif usrin == "7":
		send_cmd('\x07')
	elif usrin == "8":
		send_cmd('\x08')
	elif usrin == "9":
		send_cmd('\x09')
	elif usrin == "exit":
		send_cmd('\x01')
		exit()
	else:
		print "#user_input_eval(): Unknown (sub)command..."


##################################################################

#"main":

openSocket()
start_new_thread(recv,())

while True:
	try:
		user_input_eval(raw_input("Please enter subcommand: "))

		if rec == 1:
			#print "omg data received!!! do sth with it"
			rec = 0
	except KeyboardInterrupt:
		print "\n#Woah, STRG+C! Stopping Bot and exit..."
		send_cmd('\x01')
		udp_sock.close()
		exit()



