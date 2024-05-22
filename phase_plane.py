import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate

def modelEquation(ic, t):
    x, v = ic
    a = 0
    b = -1
    res = [v, a*v-b*x]
    return res

x_0 = 20
y_0 = -10

Y0 = [x_0, y_0]

t = np.linspace(0, 20, 1000)
solution = integrate.odeint(modelEquation, Y0, t)
x = solution[:, 0]
v = solution[:, 1]
plt.plot(x, v)
plt.axhline(0, color='black',linewidth=1)
plt.axvline(0, color='black',linewidth=1)
plt.xlabel('x')
plt.ylabel("v")
plt.title("Phase plane plot of v(x)")
plt.grid(True)
#plt.xlim(-20, 20)
#plt.ylim(-20, 20)
plt.show()