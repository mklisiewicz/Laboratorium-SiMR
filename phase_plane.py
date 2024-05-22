import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate
from mpl_toolkits import mplot3d
from scipy.integrate import quad



plt.rcParams['text.usetex'] = True
def plotPhasePlane():
    a = -2
    b = 3
    def solution1D(ic, t):
        x, v = ic
        
        res = [v, -a*v-b*x]
        return res

    def energy(x, v): 
        return -a*v-b*x + v**2/2

    x_0 = 20
    v_0 = -10

    Y0 = [x_0, v_0]

    t = np.linspace(0, 20, 1000)
    solution = integrate.odeint(solution1D, Y0, t)
    x = solution[:, 0]
    v = solution[:, 1]

    # Calculate the energy for each pair of (x, v)
    E = energy(x, v)
    print(E.min())

    # Create a 3D scatter plot
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.scatter3D(x, v, E, c=E, cmap='viridis')

    E_0 = energy(x_0, v_0)
    ax.scatter3D(x_0, v_0, E_0, c='red', s=100)

    ax.set_xlabel('$x$')
    ax.set_ylabel('$v$')
    ax.set_zlabel('$\\frac{E}{m}$')

    plt.show()

plotPhasePlane()
'''
def plotPhaseSpace(initial_conditions):
    def x_solution1D(initial_conditions, t, B, res):
        v, x, y, z = initial_conditions
        l = 5

        res[0] = v
        res[1] = B[0]*(2*l*(x/np.sqrt(x**2+(z+l)**2+y**2)+x/np.sqrt(x**2+(z-l)**2+y**2))-2*x)
        return res
    
    def y_solution1D(initial_conditions, t, B, res):
        v, x, y, z = initial_conditions
        l = 5

        res[0] = v
        res[2] = B[1]*(2*l*(y/np.sqrt(y**2+(z+l)**2+x**2)+y/np.sqrt(y**2+(z-l)**2+x**2))-2*y)
        return res
    
    def z_solution1D(initial_conditions, t, B, res):
        v, x, y, z = initial_conditions
        l = 5

        res[0] = v
        res[3] = B[2]*(2*l*((z+l)/np.sqrt((z+l)**2+y**2+x**2)+(z-l)/np.sqrt((z-l)**2+y**2+x**2))-2*z)
        return res
    B = [1, 1, 1]

    t = np.linspace(0, 20, 1000)
    x_solution = integrate.odeint(x_solution1D, initial_conditions, t, args=(B, 0))
    y_solution = integrate.odeint(y_solution1D, initial_conditions, t, args=(B, 1))
    z_solution = integrate.odeint(z_solution1D, initial_conditions, t, args=(B, 2))

    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.plot3D(x_solution[:, 1], y_solution[:, 2], z_solution[:, 3], 50, cmap='binary')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
ic = [0, 0, 0, 0]
plotPhaseSpace(ic)
plt.show()
'''



