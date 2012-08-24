func main(): None = 
	consts PI:Double = 3.14159265359;
	vars a = 0;
	
	func mem_dump:None = native
	func stack_dump:None = native
	func print_int(i:Int):None = native
	func print_double(d:Double):None = native
	func read_int:Int = native
	func read_double:Double = native	
	func sin(a:Double):Double = native
	func random():Int = native


	func min(a:Double, b:Double):Double = if a < b then a else b
	func max(a:Double, b:Double):Double = if a > b then a else b
	
	func fac(n:Int):Int = if n == 0 then 1 else fac(n-1)*n
	func fac_iter(n:Int):Int =
		vars ret = 1;
	{ 
		while n > 0 do {
			ret = ret * n;
			n = n - 1;
		};
		ret;
	}

	func randmm(min:Int,max:Int):Int = min + (random() % (max + 1 - min))

	func print_random(i:Int):None = while i > 0 do 
	{ 
		print_int(randmm(10,20));
		i = i - 1;
	}

	func print_rec(i:Int):None = if i == 0 then print_int(0) else
	{ 
		print_int(i);
		print_rec(i-1);
	}

	func guess(min:Int, max:Int):Int =
		consts number = randmm(min, max);
		vars input = 0, trys = 1;
	{
		while (input = read_int()) != number do {
			print_int(if input < number then 1 else -1);
			trys = trys + 1;
		};
		print_int(trys);
	}	

{
	guess(0,100);
}
