import numpy as np
import matplotlib.pyplot as plt

# Get user inputs for a, b
a = 1
b = 3

# Calculate delta
delta = a**2 - 4*b

# Get user inputs for initial conditions x(0) and x'(0)
Y0 = [5, 5]

# Define x range
x = np.linspace(-20, 20, 1000)

theta = np.linspace(0, 2*np.pi, 1000)

if a == 0 and b > 0 and delta < 0:
    # Case 1: x'^2 + bx^2 = c
    c = Y0[0]**2 + b*Y0[1]**2
    r = np.sqrt(c / b)  # radius of the circle
    x = r * np.cos(theta)
    xp = r * np.sin(theta)
    plt.plot(x, xp)
elif a != 0 and delta < 0:
    # Case 2: r_{1, 2}= -1/2a+-i omega
    omega = np.sqrt(-delta)
    r1 = -1/(2*a) + 1j*omega
    r2 = -1/(2*a) - 1j*omega
    # Define t range
    t = np.linspace(0, 50, 5000)
    # Solve for C1 and C2 using the initial conditions
    C1 = Y0[0]
    C2 = (Y0[1] - np.real(r1)*C1) / -np.imag(r1)
    # Solve for x and x' using the roots r1 and r2
    x = np.exp(np.real(r1)*t) * (C1*np.cos(np.imag(r1)*t) + C2*np.sin(np.imag(r1)*t))
    xp = np.real(r1) * x + np.exp(np.real(r1)*t) * (C1*np.sin(np.imag(r1)*t) + C2*np.cos(np.imag(r1)*t))
    plt.plot(x, xp)
elif a != 0 and delta >= 0:
    # Case 3: r_{1, 2}= -1/2a+-omega
    omega = np.sqrt(delta)
    r1, r2 = -1/(2*a) + omega, -1/(2*a) - omega
    # Solve for x' using the roots r1 and r2
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
plt.xlim(-20, 20)  # Set x limits
plt.ylim(-20, 20)  # Set y limits
plt.show()