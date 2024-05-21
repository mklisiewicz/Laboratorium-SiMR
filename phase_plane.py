import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate


def plotPhasePlane(a, b, Y0):
    delta = a**2 - 4*b

    if a == 0 and b > 0 and delta < 0:
        theta = np.linspace(0, 2*np.pi, 1000)
        c = Y0[0]**2 + b*Y0[1]**2
        r = np.sqrt(c / b)
        x = r * np.cos(theta)
        xp = r * np.sin(theta)
        plt.plot(x, xp)

    elif a != 0 and delta < 0:
        omega = np.sqrt(-delta)
        r1 = -1/(2*a) + 1j*omega
        r2 = -1/(2*a) - 1j*omega

        t = np.linspace(0, 10, 500)

        C1 = Y0[0]
        C2 = (Y0[1] - np.real(r1)*C1) / -np.imag(r1)

        x = np.exp(np.real(r1)*t) * (C1*np.cos(np.imag(r1)*t) + C2*np.sin(np.imag(r1)*t))
        xp = np.real(r1) * x + np.exp(np.real(r1)*t) * (C1*np.sin(np.imag(r1)*t) + C2*np.cos(np.imag(r1)*t))
        plt.plot(x, xp)

    elif a != 0 and delta >= 0:
        x = np.linspace(-20, 20, 1000)
        omega = np.sqrt(delta)
        r1, r2 = -1/(2*a) + omega, -1/(2*a) - omega
        xp = r1*x + r2*x
        plt.plot(x, xp)
        
    elif a == 0 and b < 0:
        x = np.linspace(0, 20, 20000)
        c = -b*Y0[0]**2 - Y0[1]**2
        d=-b*x**2 - c
        print(d)
        v = np.sqrt(d)
        plt.plot(x, v)
        plt.plot(x, -v)

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