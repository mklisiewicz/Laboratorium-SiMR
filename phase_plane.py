import numpy as np
import matplotlib.pyplot as plt
from scipy import integrate
from mpl_toolkits import mplot3d
from matplotlib.animation import FFMpegWriter, PillowWriter
from matplotlib import cm
from concurrent.futures import ProcessPoolExecutor


plt.rcParams['text.usetex'] = True
fig, ax = plt.subplots(subplot_kw=dict(projection='3d'))


def solvePhaseTrajectory(a, b):
    def solution1D(ic, t):
        x, v = ic
        res = [v, -a*v-b*x]
        return res

    def energy(x, v): 
        return (b*x**2+v**2)/2

    x_0 = 15
    v_0 = 0

    Y0 = [x_0, v_0]
    E_0 = energy(x_0, v_0)

    t = np.linspace(0, 10, 2000)
    solution = integrate.odeint(solution1D, Y0, t)
    x = solution[:, 0]
    v = solution[:, 1]
    E = energy(x, v)
   
    return x, v, E, x_0, v_0, E_0

xvec = np.linspace(-15, 15, 1000)
yvec = np.linspace(-15, 15, 1000)

xlist, ylist = np.meshgrid(xvec, yvec)

# writer = FFMpegWriter(fps=15)

def plotPhaseTrajectory(a, b):
    energy_surface = (b*xlist**2+ylist**2)/2
    delta = a**2 - 4*b
    x, v, E, x_0, v_0, E_0 = solvePhaseTrajectory(a, b)
    ax.set_xlabel('$x$')
    ax.set_ylabel('$v$')
    ax.set_zlabel('$\\frac{E}{m}$')
    ax.text2D(0.05, 0.95, f"$2h = {a}$", transform=ax.transAxes)
    ax.text2D(0.05, 0.85, f"$\omega_0^2 = {b}$", transform=ax.transAxes)
    ax.text2D(0.05, 0.75, f"$\Delta = {delta}$", transform=ax.transAxes)
    ax.text2D(0.05, 0.65, f"$x_0 = {x_0}$", transform=ax.transAxes)
    ax.text2D(0.05, 0.55, f"$v_0 = {v_0}$", transform=ax.transAxes)

    z_min = 0
    z_max = E.max() * 2
    ax.set_xlim([-30, 30])
    ax.set_ylim([-30, 30])
    ax.set_zlim([z_min, E.max() * 2])
    ax.scatter(x_0, v_0, E_0, c='red', s=100, zorder=1)
    ax.scatter(x,v,E, c=E, cmap='viridis', zorder=2)
    ax.plot_surface(xlist,ylist, energy_surface, alpha=0.35, cmap=cm.viridis, zorder=3) 
    # plt.savefig(f"./plots/phase_plane_{a}_{b}.png")
    plt.show()

if __name__ == '__main__':
    # import sys
    # a = float(sys.argv[1])
    # b = float(sys.argv[2])
    a=0.5
    b=3
    plotPhaseTrajectory(a, b)



