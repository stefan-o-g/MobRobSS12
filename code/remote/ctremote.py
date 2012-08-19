#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  ctremote.py  - Skript um den ctBot per WLAN fernzusteuern...
#  
#  Copyright 2012
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#

from sys import argv, exit
from os import system
import socket
import thread
from time import sleep
from struct import *

#################################################################
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

#EOF CODE TAKEN FROM: http://code.activestate.com/recipes/134892/
#################################################################

rec_data = ""		#buffer for data received from the bot
rec_size = 1024		#max size of buffer (rec_data)

udp_sock = 0 					#socket for sending data
udp_bot_ip = "192.168.0.255"	#ip of the bot
udp_port = 10002				#port

prompt = "ctBot-remote $ "



def help(cmd=""):
	'''Printing help for the user.'''
	if cmd == "":
		print "COMMANDS:"
		print "subcmd <subcommand>"
		print "   Send a subcommand to the Bot."
		print "move"
		print "   Control the bot using 'W A S D'."
		print "get <what>"
		print "   Get the value of <what>."
		print "set <what> <value>"
		print "   Set <what> to <value>."
		print "   <what>: botip : The IP of the bot."
		print "           port  : The port the bot is using."
		print "tokensend <tokenfile>"
		print "   Sending a tokenfile to the Bot."
		print "help"
		print "   Print this."
		print "help <cmd>"
		print "   Print help for a single command."
		print "exit"
		print "(q)uit"
		print "   Exit this script."
		print "Strg+c"
		print "   Stops the bot."
	elif cmd == "subcmd":
		print "subcmd <subcommand>"
		print "   Send a subcommand to the bot."
		print "<subcommand> can be one of the following:"
		print "   1 or stand : The bot will just stop."
		print "   2 or motte1 : The bot will execute 'light(1)'."
		print "   3 or kakerlake1 : The bot will execute 'light(-1)'."
		print "   4 or motte2 : The bot will execute 'motte_nonbehav()'."
		print "   5 or kakerlake2 : The bot will execute 'kakerlake_nonbehav()'."
		print "   6 or acht : The bot will execute 'acht_nonbehav()'."
		print "   7 or linie : The bot will execute 'linie()'."
	elif cmd == "get":
		print "get <what>"
		print "   Get the value of <what>."
		print "<what> can be one of the following:"
		print "   botip : The IP of the bot."
		print "   port  : The port the bot is using."
	elif cmd == "set":
		print "set <what> <value>"
		print "   Set <what> to <value>."
		print "<what> can be one of the following:"
		print "   botip : The IP of the bot."
		print "      <value> can be any valid IPv4 address."
		print "   port  : The port the bot is using."
		print "      <value> can be any valid port. (0-65535)"
	elif cmd == "help":
		print "help <cmd>"
		print "   Print help for a single command."
		print "<cmd> can be one of the following:"
		print "   subcmd\n   set\n   get\n   tokensend\n   help"
	elif cmd == "tokensend":
		print "tokensend <tokenfile>"
		print "   <tokenfile> has to look like this:"
		print "      token value"
		print "   Example:"
		print "      5 while"
		print "      7 a"
		print "      45 ="
		print "      7 b"
		print "   The bot will compile and interpret the code/tokens."
	else:
		print "#Sorry. No help available for this command..."

##################################################################

def openSocket():
	'''Open a UDP-Socket to send data to the bot.'''
	global udp_sock, udp_ip, udp_port

	print "#Opening socket for new connection..."
	print "#Using bot-IP ", udp_bot_ip
	print "#Using port ", udp_port 

	udp_sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
	split_ip = udp_bot_ip.split(".")
	if split_ip[3] == "255":
		print "#Using broadcast..."
		udp_sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
	udp_sock.sendto( "Hello ctBot!", (udp_bot_ip, udp_port) )

##################################################################

def send_cmd(subcmd, ldata="ld", rdata="rd", payload="\x00", data=""):
	'''Builds and sends a command to the bot.
	A command looks like this:
	start (>) + 1byte command  + 1byte subcommand + 1byte payload 
	+ 2byte left data + 2byte right data + 2byte seq nr + end (<) + data
	Payload is the size of the 'data' appended to the command in bytes.'''

	#if type(subcmd) == int:
	#	subcmd = str(subcmd)
	#if type(ldata) == int:
	#	ldata = str(ldata)
	#if type(rdata) == int:
	#	rdata = str(rdata)
	#if type(payload) == int:
	#	payload = str(payload)
	#if type(data) == int:
	#	data = str(data)

	#start (>) + 1byte command  + 1byte subcommand + 1byte payload 
	#+ 2byte left data + 2byte right data + 2byte seq nr + end (<) + data
	cmd = ">#" + subcmd + payload + ldata + rdata + "sn<"	+ data

	#if len(cmd) != 11:
	#	print "#Error: wrong paket length!"
	#	return

	global udp_sock, udp_bot_ip, udp_port
	udp_sock.sendto( cmd, (udp_bot_ip, udp_port) )

##################################################################

def recv():
	'''Running in a thread, receiving the data from the bot an processing it.'''
	global rec, rec_data, rec_size, prompt, run

	sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM )
	sock.bind( ("",10002) )

	while run:
		rec_data = sock.recv(rec_size)
		if rec_data != "":
			#print "\n#Data received...\n" + prompt
			#TODO: do something with rec_data here
			rec_data = ""
	sock.close()

##################################################################

def user_input_eval(usrin):
	'''Parses the user input and performs correspondig actions.'''
	cmd_list = usrin.split()
	global udp_bot_ip
	global udp_port
	global run

	try:
		if cmd_list[0] == "subcmd":
			#SUBCMD
			if cmd_list[1] == "1" or cmd_list[1] == "stand":
				send_cmd('\x01')
			elif cmd_list[1] == "2" or cmd_list[1] == "motte1":
				send_cmd('\x02')
			elif cmd_list[1] == "3" or cmd_list[1] == "kakerlake1":
				send_cmd('\x03')
			elif cmd_list[1] == "4" or cmd_list[1] == "motte2":
				send_cmd('\x04')
			elif cmd_list[1] == "5" or cmd_list[1] == "kakerlake2":
				send_cmd('\x05')
			elif cmd_list[1] == "6" or cmd_list[1] == "acht":
				send_cmd('\x06')
			elif cmd_list[1] == "7" or cmd_list[1] == "linie":
				send_cmd('\x07')
			elif cmd_list[1] == "8":
				send_cmd('\x08')
			elif cmd_list[1] == "9":
				send_cmd('\x09')
			else:
				print "\n#Unknown subcommand... Try 'help'..."
			#EOF SUBCMD
		elif cmd_list[0] == "move":
			move()
		elif cmd_list[0] == "set":
			#SET
			if cmd_list[1] == "botip":
				try:
					socket.inet_aton(cmd_list[2])
					if len(cmd_list[2].split(".")) != 4:
						raise
					udp_bot_ip = cmd_list[2]
					print "#New bot-IP: " + udp_bot_ip
					openSocket()
				except:
					print "#Invalid IP!"
			elif cmd_list[1] == "port":
				newport = int(cmd_list[2])
				if newport >= 0 and newport <= 65535:
					udp_port = int(cmd_list[2])
					print "#New port: ", udp_port
					openSocket()
				else:
					print "#Invalid port!"
			else:
				print "#Dont know what to set..."
			#EOF SET
		elif cmd_list[0] == "get":
			#GET
			if cmd_list[1] == "botip":
				print "#bot-IP: " + udp_bot_ip
			elif cmd_list[1] == "port":
				print "#port: ", udp_port
			else:
				print "#Dont know what you want to get..."
			#EOF GET
		elif cmd_list[0] == "tokensend":
			tokensend(cmd_list[1])	
		elif cmd_list[0] == "exit" or cmd_list[0] == "quit" or cmd_list[0] == "q":
			send_cmd('\x01')
			run = False
		elif cmd_list[0] == "help":
			if len(cmd_list) == 2:
				help(cmd_list[1])
			else:
				help()
		else:
			print "\n#Unknown command... Try 'help'..."
	except Exception, e:
		print "\n#Command ERROR! Try 'help'..."
		print "#Error: ", e
		#import traceback
		#print traceback.format_exc()
		


##################################################################

def move():
	'''Controls the bot using the 'wasd'-keys.'''
	print "Bot controlls:"
	#print "w -> foward		q -> stop 'move'"
	#print "s -> backward		e -> stop Bot"
	#print "a -> turn left"
	#print "d -> turn right"
	print "                 forward"
	print "                 v"
	print "stop 'move' > Q  W  E < stop Bot"
	print "  turn left > A  S  D < turn right"
	print "                 ^"
	print "                 backward"
	getch = _Getch()
	foo = getch()
	while foo != 'q':
		if foo == 'w':
			send_cmd('m',"\xff\x00","\xff\x00")
			send_cmd('m',"\xff\x00","\xff\x00")
			send_cmd('m',"\xff\x00","\xff\x00")
		elif foo == 's':
			send_cmd('m',"\x10\x80","\x10\x80")
			send_cmd('m',"\x10\x80","\x10\x80")
			send_cmd('m',"\x10\x80","\x10\x80")
		elif foo == 'a':
			send_cmd('m',"\x10\x80","\x10\x00")
			send_cmd('m',"\x10\x80","\x10\x00")
			send_cmd('m',"\x10\x80","\x10\x00")
		elif foo == 'd':
			send_cmd('m',"\x10\x00","\x10\x80")
			send_cmd('m',"\x10\x00","\x10\x80")
			send_cmd('m',"\x10\x00","\x10\x80")
		elif foo == 'e':
			send_cmd('m',"\x00\x00","\x00\x00")
			send_cmd('m',"\x00\x00","\x00\x00")
			send_cmd('m',"\x00\x00","\x00\x00")
		foo = getch()

	send_cmd('m',"\x00\x00","\x00\x00")
	send_cmd('m',"\x00\x00","\x00\x00")
	send_cmd('m',"\x00\x00","\x00\x00")
		

##################################################################

def tokensend(infile=""):
	'''Sending the given "tokenfile" to the Bot.
	The transmission ands with an l/r-data = 0xFF paket.
	The token is transmitted in ldata and rdata to have redundacy.
	The "tokenfile" has to look like this:
	token value
	Example:
	5 while'''
	if infile == "":
		print "#Error: no file given!"
		return

	try:
		fileobject = open(infile, "r")
	except:
		print "#Error: no such file!"
		return
	
	try:
		print "#Sending... this could take a while..."
		send_cmd('t') #START transmission
		for line in fileobject:
			tmpln = line.split(" ")
			try:
				token = int(tmpln[0])
				#get token as little end. unsig. short 
				tokenpak = pack('<H', token)
			except:
				print "#Error: token is not a number!"
				fileobject.close()
				return
			try:
				#get payload as little end. unsig. char
				value = tmpln[1].strip()
				pload = pack('<B', len(value))
			except:
				print "#Error: payload error!"
				fileobject.close()
				return
			send_cmd('t', tokenpak, tokenpak, pload, tmpln[1])
			sleep(1)
		#send_cmd('t', "\xff\xff", "\xff\xff") #EOF transmission

	except:
		print "#Error: bad file!"
		fileobject.close()
		return

	fileobject.close()



##################################################################

#"main":
print "ctremote.py - Remote control your ct'Bot!"
print "Enter 'help' to get some help ;)"
openSocket()
thread.start_new_thread(recv,())

run=True
while run:
	try:
		user_input_eval(raw_input(prompt))

	except KeyboardInterrupt:
		print "\n#Panic, STRG+C! Stopping Bot..."
		print "(To exit ctremote.py use 'exit' or '(q)uit')"
		send_cmd('\x01')
		send_cmd('\x01')
		send_cmd('\x01')

udp_sock.close()


