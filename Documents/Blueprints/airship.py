x = 0.11

Pi = 3.14
g = 9.8
Rho_O = 1.23
Rho_He = 0.178
a = x
b = x
c = x*4
V = Pi*(a*b*c)*4/3


Fa = (Rho_O - Rho_He)*V

vl = V*10**3
fg = Fa*1000
print("V = " + str(vl) + "[l]")
print("Fa = " + str(fg) + "[g]")
print("z = " + str(c*2) + "[m]")
