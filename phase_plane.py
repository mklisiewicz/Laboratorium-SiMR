import numpy as np
import matplotlib.pyplot as plt

a = 1
b = 3

delta = a**2 - 4*b

Y0 = [5, -5]

theta = np.linspace(0, 2*np.pi, 1000)

if a == 0 and b > 0 and delta < 0:
    c = Y0[0]**2 + b*Y0[1]**2
    r = np.sqrt(c / b)
    x = r * np.cos(theta)
    xp = r * np.sin(theta)
    plt.plot(x, xp)

elif a != 0 and delta < 0:
    omega = np.sqrt(-delta)
    r1 = -1/(2*a) + 1j*omega
    r2 = -1/(2*a) - 1j*omega

    t = np.linspace(0, 50, 5000)

    C1 = Y0[0]
    C2 = (Y0[1] - np.real(r1)*C1) / -np.imag(r1)

    x = np.exp(np.real(r1)*t) * (C1*np.cos(np.imag(r1)*t) + C2*np.sin(np.imag(r1)*t))
    xp = np.real(r1) * x + np.exp(np.real(r1)*t) * (C1*np.sin(np.imag(r1)*t) + C2*np.cos(np.imag(r1)*t))
    plt.plot(x, xp)
elif a != 0 and delta >= 0:
    omega = np.sqrt(delta)
    r1, r2 = -1/(2*a) + omega, -1/(2*a) - omega

    xp = r1*x + r2*x
    plt.plot(x, xp)
elif a == 0 and b < 0:
    c = -b*Y0[0]**2 - Y0[1]**2
    v = np.sqrt(-b*x**2 - c)
    plt.plot(x, v, 'r')
    plt.plot(x, -v, 'r')

plt.axhline(0, color='black',linewidth=1)
plt.axvline(0, color='black',linewidth=1)
plt.xlabel('x')
plt.ylabel("v")
plt.title("Phase plane plot of v(x)")
plt.grid(True)
plt.xlim(-20, 20)
plt.ylim(-20, 20)
plt.show()