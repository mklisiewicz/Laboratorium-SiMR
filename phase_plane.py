import numpy as np
import matplotlib.pyplot as plt

# Get user inputs for a, b
a = 0
b = -2

# Calculate delta
delta = a**2 - 4*b

# Get user inputs for initial conditions x(0) and x'(0)
Y0 = [5, 2]

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
    r1, r2 = -1/(2*a) + 1j*omega, -1/(2*a) - 1j*omega
    # Define t range
    t = np.linspace(0, 10, 1000)
    # Solve for x and x' using the roots r1 and r2
    x = np.exp(np.real(r1)*t) * (Y0[0]*np.cos(np.imag(r1)*t) - Y0[1]*np.sin(np.imag(r1)*t))
    xp = np.real(r1) * x + np.exp(np.real(r1)*t) * (Y0[0]*np.sin(np.imag(r1)*t) + Y0[1]*np.cos(np.imag(r1)*t))
    plt.plot(x, xp)
elif a != 0 and delta >= 0:
    # Case 3: r_{1, 2}= -1/2a+-omega
    omega = np.sqrt(delta)
    r1, r2 = -1/(2*a) + omega, -1/(2*a) - omega
    # Solve for x' using the roots r1 and r2
    xp = r1*x + r2*x
    plt.plot(x, xp)
elif a == 0 and b < 0:
    x = np.linspace(-9, 9, 400)
    c = -b*Y0[0]**2 - Y0[1]**2
    values = -b*x**2 - c
    pos = values >= 0
    neg = values < 0
    v_pos = np.sqrt(values[pos])
    v_neg = np.sqrt(-values[neg])
    plt.plot(x[pos], v_pos, 'r')
    #plt.plot(x[neg], v_neg, 'r')


plt.xlabel('x')
plt.ylabel("v")
plt.title("Phase plane plot of v(x)")
plt.grid(True)
plt.xlim(-20, 20)  # Set x limits
plt.ylim(-20, 20)  # Set y limits
plt.show()