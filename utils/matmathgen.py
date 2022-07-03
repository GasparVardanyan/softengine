#!/usr/bin/env python3

print ("matrix4 multiplication:")

for i in range (4) :
	for j in range (4) :
		s = ""
		for k in range (4) :
			s += " + m1.m" + str (i) + str (k) + " * m2.m" + str (k) + str (j)
		print (s [3:] + ",")

print ("\n\n\n\n")



print ("matrix4 transform:")

rx = [
		["1", "0", "0", "0"],
		["0", "cx", "sx", "0"],
		["0", "-sx", "cx", "0"],
		["0", "0", "0", "1"]
]
ry = [
		["cy", "0", "-sy", "0"],
		["0", "1", "0", "0"],
		["sy", "0", "cy", "0"],
		["0", "0", "0", "1"]
]
rz = [
		["cz", "sz", "0", "0"],
		["-sz", "cz", "0", "0"],
		["0", "0", "1", "0"],
		["0", "0", "0", "1"]
]

def mmul (a, b) :
	m = []
	for i in range (4) :
		r = []
		for j in range (4) :
			s = ""
			for k in range (4) :
				if a [i] [k] == "0" or b [k] [j] == "0" :
					pass
				elif a [i] [k] == 1 and b [k] [j] == 1 :
					s += " + 1"
				elif a [i] [k] == "1" :
					s += " + " + b [k] [j]
				elif b [k] [j] == "1" :
					s += " + " + a [i] [k]
				else :
					s += " + (" + a [i] [k] + ") * (" + b [k] [j] + ")"
			r.append ("0" if s == "" else s [3:])
		m.append (r)
	return m

m = mmul (rz, mmul (ry, rx))



for i in range (3) :
	for j in range (3) :
		r = m [i] [j]
		print ("scl." + ["x", "y", "z"] [j % 3] + " * (" + r + "),")
	print ("0,")
print ("pos.x, pos.y, pos.z, 1,")
