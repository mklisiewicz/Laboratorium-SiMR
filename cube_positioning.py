import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def getPotentialEnergyEquation(spring_elongations, spring_constants):

    return b*x**2/2

def solveCubeXTrajectory(k, m):
    def solution1D(ic, t):
        x, v_x = ic
        res = [x, -a*v-b*x]
        return res

    def energy(x, v): 
        return (b*x**2+v**2)/2

    x_0 = 5
    v_0 = 2

    Y0 = [x_0, v_0]
    E_0 = energy(x_0, v_0)

    t = np.linspace(0, 10, 2000)
    solution = integrate.odeint(solution1D, Y0, t)
    x = solution[:, 0]
    v = solution[:, 1]
    E = energy(x, v)
   
    return x, v, E, x_0, v_0, E_0

def plotCubePosition(normal_vector, P_0, cube_axes, points):
    L_x, L_z = cube_axes
    L_xl, L_xr = L_x
    L_zb, L_zt = L_z

    x_kl, x_kr, z_kb, z_kt = points
    A, B, C = normal_vector
    x = np.linspace(-10, 10, 100)
    y = np.linspace(-10, 10, 100)
    x, y = np.meshgrid(x, y)

    z = (-A*x - B*y)/C

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    #ax.plot_surface(x, y, z, color='b', alpha=0.5)

    ax.quiver(0, 0, 0, 1, 0, 0, color='r', length=5)  # x-axis
    ax.quiver(0, 0, 0, 0, 1, 0, color='g', length=5)  # y-axis
    ax.quiver(0, 0, 0, 0, 0, 1, color='b', length=5)  # z-axis

    for point, color in zip(points, ['g', 'y', 'c', 'm']):
        ax.scatter(*point, color=color)

    ax.scatter(L_x[:, 0], L_x[:, 1], L_x[:, 2], color='r')
    ax.scatter(L_z[:, 0], L_z[:, 1], L_z[:, 2], color='b')

    ax.plot([L_xr[0], L_xl[0]], [L_xr[1], L_xl[1]], [L_xr[2], L_xl[2]], color='r')
    ax.plot([L_zt[0], L_zb[0]], [L_zt[1], L_zb[1]], [L_zt[2], L_zb[2]], color='b')

    ax.plot([x_kl[0], L_xl[0]], [x_kl[1], L_xl[1]], [x_kl[2], L_xl[2]], color='g')
    ax.plot([x_kr[0], L_xr[0]], [x_kr[1], L_xr[1]], [x_kr[2], L_xr[2]], color='y')
    ax.plot([z_kb[0], L_zb[0]], [z_kb[1], L_zb[1]], [z_kb[2], L_zb[2]], color='c')
    ax.plot([z_kt[0], L_zt[0]], [z_kt[1], L_zt[1]], [z_kt[2], L_zt[2]], color='m')

    ax.set_xlim([-35, 35])
    ax.set_ylim([-35, 35])
    ax.set_zlim([-35, 35])
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()

def getCubeAxes(vectors, P_0, rotation_matrix):
    L_x_rotated = rotation_matrix @ vectors[0]   
    L_z_rotated = rotation_matrix @ vectors[1] 

    L_xr_rotated = P_0 + L_x_rotated
    L_xl_rotated = P_0 - L_x_rotated
    L_zt_rotated = P_0 + L_z_rotated
    L_zb_rotated = P_0 - L_z_rotated

    print(np.linalg.norm(L_xr_rotated - L_zt_rotated))
    print(np.linalg.norm(L_xr_rotated - L_zb_rotated))

    is_orthogonal = np.dot(L_x_rotated, L_z_rotated)
    print(is_orthogonal)
    if is_orthogonal > 1e-14:
        raise ValueError("L_x and L_z are not orthogonal")

    L_x_axis = np.array([L_xl_rotated, L_xr_rotated])
    L_z_axis = np.array([L_zb_rotated, L_zt_rotated])

    return L_x_axis, L_z_axis

def getNormalVector(vectors, rotation_matrix, P_0):
    n=rotation_matrix @ np.cross(vectors[0] + P_0, vectors[1] + P_0)
    return n

def getRotationMatrix(rotation_angles):
    alpha, beta, gamma = rotation_angles

    #  Intrinsic rotation whose Tait–Bryan angles are α, β, γ, about axes z, y, x, respectively
    R = np.array([[np.cos(gamma)*np.cos(beta), 
                   np.cos(gamma)*np.sin(beta)*np.sin(alpha)-np.sin(gamma)*np.cos(alpha), 
                   np.cos(gamma)*np.sin(beta)*np.cos(alpha)+np.sin(gamma)*np.sin(alpha)],
                  [np.sin(gamma)*np.cos(beta), 
                   np.sin(gamma)*np.sin(beta)*np.sin(alpha)+np.cos(gamma)*np.cos(alpha), 
                   np.sin(gamma)*np.sin(beta)*np.cos(alpha)-np.cos(gamma)*np.sin(alpha)],
                  [-np.sin(beta), 
                   np.cos(beta)*np.sin(alpha), 
                   np.cos(beta)*np.cos(alpha)]])
    return R

if __name__ == '__main__':
    L_vector_length = 10

    # alpha = np.pi/3
    # beta = np.pi/4
    # gamma = np.pi/6

    alpha = 0
    beta = 0
    gamma = 0

    x_kr = np.array([30, 0, 0])
    x_kl = np.array([-30, 0, 0])
    z_kt = np.array([0, 0, 30])
    z_kb = np.array([0, 0, -30])

    spring_attachment_points = [x_kl, x_kr, z_kb, z_kt]
    
    x_0 = 6
    y_0 = 3
    z_0 = -4

    L_x_vec = L_vector_length*np.array([1, 0, 0])
    L_z_vec = L_vector_length*np.array([0, 0, 1])

    L_vectors = np.array([L_x_vec, L_z_vec])
    P_0 = np.array([z_0, y_0, x_0])
    rotation_angles = [alpha, beta, gamma]
    rotation_matrix = getRotationMatrix(rotation_angles)
    normal_vector = getNormalVector(L_vectors, rotation_matrix, P_0)
    plotCubePosition(normal_vector, P_0, getCubeAxes(L_vectors, P_0, rotation_matrix), spring_attachment_points)