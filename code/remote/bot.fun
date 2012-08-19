func main(): None =
	vars pos:Int;	

	func print_int(i:Int):None = native
	func motor_set(l:Int, r:Int):None = native
	func sens_LDRL():Int = native
	func sens_LDRR():Int = native	

while 1 do {
	pos = sens_LDRL();
	print_int(pos);
	if 1 then motor_set(150, -150);
}
