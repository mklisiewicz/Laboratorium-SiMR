import sympy as sp
import numpy as np
from scipy.integrate import solve_ivp
from scipy.integrate import odeint
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
from matplotlib.animation import FFMpegWriter
from sympy import pprint
import cube_positioning as cp



# Define symbols and functions
t, g, m, k, l_0 = sp.symbols('t g m k l_0')
x, y, z = sp.symbols('x y z', cls=sp.Function)
alpha, beta, gamma = sp.symbols(r'a b g', cls=sp.Function)

# Define time-dependent functions
alpha = alpha(t)
beta = beta(t)
gamma = gamma(t)
alpha_dot = sp.diff(alpha, t)
beta_dot = sp.diff(beta, t)
gamma_dot = sp.diff(gamma, t)
alpha_ddot = sp.diff(alpha_dot, t)
beta_ddot = sp.diff(beta_dot, t)
gamma_ddot = sp.diff(gamma_dot, t)

x=x(t)
y=y(t)
z=z(t)
x_dot = sp.diff(x, t)
y_dot = sp.diff(y, t)
z_dot = sp.diff(z, t)
x_ddot = sp.diff(x_dot, t)
y_ddot = sp.diff(y_dot, t)
z_ddot = sp.diff(z_dot, t)

# Define rotation matrices as functions of time
def get_rotation_matrix(alpha, beta, gamma):
    R_x = sp.Matrix([
        [1, 0, 0],
        [0, sp.cos(alpha), -sp.sin(alpha)],
        [0, sp.sin(alpha), sp.cos(alpha)]
    ])

    R_y = sp.Matrix([
        [sp.cos(beta), 0, sp.sin(beta)],
        [0, 1, 0],
        [-sp.sin(beta), 0, sp.cos(beta)]
    ])

    R_z = sp.Matrix([
        [sp.cos(gamma), -sp.sin(gamma), 0],
        [sp.sin(gamma), sp.cos(gamma), 0],
        [0, 0, 1]
    ])

    return R_z * R_y * R_x

# Get the combined rotation matrix
R = get_rotation_matrix(alpha, beta, gamma)

cube_k_vector_length = 30
vector_kr = sp.Matrix([1, 0, 0])*cube_k_vector_length
vector_kl = sp.Matrix([-1, 0, 0])*cube_k_vector_length
vector_kt = sp.Matrix([0, 0, 1])*cube_k_vector_length
vector_kb = sp.Matrix([0, 0, -1])*cube_k_vector_length

kr = sp.Matrix([100, 0, 0])
kl = sp.Matrix([-100, 0, 0])
kt = sp.Matrix([0, 0, 100])
kb = sp.Matrix([0, 0, -100])

# Positions of the points in space
p = sp.Matrix([x, y, z])
p_kr = p + R * vector_kr
p_kl = p + R * vector_kl
p_kt = p + R * vector_kt
p_kb = p + R * vector_kb
print(p_kr)

distance_kr = sp.sqrt((p_kr - kr).dot(p_kr - kr))
distance_kl = sp.sqrt((p_kl - kl).dot(p_kl - kl))
distance_kt = sp.sqrt((p_kt - kt).dot(p_kt - kt))
distance_kb = sp.sqrt((p_kb - kb).dot(p_kb - kb))

T_trans = (1/2) * m * (x_dot**2 + y_dot**2 + z_dot**2)


omega = sp.Matrix([alpha_dot, beta_dot, gamma_dot])
I = sp.diag(1, 1, 1)  # Assuming unit inertia tensor for simplicity
T_rot = (1/2) * omega.dot(I * omega)

T = T_trans + T_rot

# Potential Energy (V)
# Spring potential energy

V_springs = (1/2) * k * ((distance_kr - l_0)**2 + (distance_kl - l_0)**2 + (distance_kt - l_0)**2 + (distance_kb - l_0)**2)

# Gravitational potential energy
V_gravity = m * g * p[2]

# V = V_springs + V_gravity
V = V_springs 

# Lagrangian
L = T - V




# latex_L = sp.latex(L)
# print("Lagrangian:")
# print(L)
# print(latex_L)

# with open("lagrangian.tex", "w") as file:
#     file.write(latex_L)


q = sp.Matrix([x, y, z, alpha, beta, gamma])
q_dot = sp.Matrix([x_dot, y_dot, z_dot, alpha_dot, beta_dot, gamma_dot])
q_ddot = sp.Matrix([x_ddot, y_ddot, z_ddot, alpha_ddot, beta_ddot, gamma_ddot])
LE_x = sp.diff(L, x) - sp.diff(sp.diff(L, x_dot), t).simplify()
LE_y = sp.diff(L, y) - sp.diff(sp.diff(L, y_dot), t).simplify()
LE_z = sp.diff(L, z) - sp.diff(sp.diff(L, z_dot), t).simplify()
LE_alpha = sp.diff(L, alpha) - sp.diff(sp.diff(L, alpha_dot)).simplify()
LE_beta = sp.diff(L, beta) - sp.diff(sp.diff(L, beta_dot)).simplify()
LE_gamma = sp.diff(L, gamma) - sp.diff(sp.diff(L, gamma_dot)).simplify()
sols = sp.solve([LE_x, LE_y, LE_z, LE_alpha, LE_beta, LE_gamma], (x_ddot, y_ddot, z_ddot, alpha_ddot, beta_ddot, gamma_ddot), simplify=False, rational=False)
# lagrange_eqns = [sp.diff(L, q[i]) - sp.diff(sp.diff(L, q_dot[i]), t).simplify() for i in range(len(q))]
# sols = sp.solve(lagrange_eqns, q_ddot)

# for eq in lagrange_eqns:
#     with open("lagrange_eqns.txt", "a") as file:
#         file.write(str(eq) + "\n\n")

for i, (key, value) in enumerate(sols.items()):
    with open(f"solution_{key}.txt", "w") as file:
        file.write(str(value))

# print(lagrange_eqns)
# latex_eqns = [sp.latex(eq) for eq in lagrange_eqns]

# print("\nEquations of Motion:")
# for i, eq in enumerate(latex_eqns):
#     print(f"Equation {i+1}:")
#     print(eq)
#     with open(f"eq_motion_{i+1}.tex", "w") as file:
#         file.write(eq)

# selected_keys = [x_ddot, y_ddot, z_ddot] 
# for key in selected_keys:
#     if key in sols:
#         pprint(f"{key}: {sols[key]}")



a_alpha = sp.lambdify([t, g, m, k, l_0, q, q_dot], sols[alpha_ddot])
a_beta = sp.lambdify([t, g, m, k, l_0, q, q_dot], sols[beta_ddot])
a_gamma = sp.lambdify([t, g, m, k, l_0, q, q_dot], sols[gamma_ddot])
v_alpha = sp.lambdify(alpha_dot, alpha_dot)
v_beta = sp.lambdify(beta_dot, beta_dot)
v_gamma = sp.lambdify(gamma_dot, gamma_dot)
a_x = sp.lambdify([t, g, m, k, l_0, q, q_dot], sols[x_ddot])
a_y = sp.lambdify([t, g, m, k, l_0, q, q_dot], sols[y_ddot])
a_z = sp.lambdify([t, g, m, k, l_0, q, q_dot], sols[z_ddot])
v_x = sp.lambdify(x_dot, x_dot)
v_y = sp.lambdify(y_dot, y_dot)
v_z = sp.lambdify(z_dot, z_dot)
# print(sols)


def f(t, y, g, m, k, l_0):
    q = y[:6]
    q_dot = y[6:]
    return [v_x(q_dot[0]), 
             v_y(q_dot[1]), 
             v_z(q_dot[2]), 
             v_alpha(q_dot[3]), 
             v_beta(q_dot[4]), 
             v_gamma(q_dot[5]),
             a_x(t, g, m, k, l_0, q, q_dot),
             a_y(t, g, m, k, l_0, q, q_dot),
             a_z(t, g, m, k, l_0, q, q_dot),
             a_alpha(t, g, m, k, l_0, q, q_dot),
             a_beta(t, g, m, k, l_0, q, q_dot),
             a_gamma(t, g, m, k, l_0, q, q_dot)]

t_span = [0, 20]
t_eval = np.linspace(*t_span, 1000)

q_ic = np.array([0, 5, 5, 0, 0, 0])
q_dot_ic = np.array([0, 0, 0, 0, 0, 0])
g = 9.81
m = 1
k = 20
l_0 = 40

kr_0 = np.array([100, 0, 0])
kl_0 = np.array([-100, 0, 0])
kt_0 = np.array([0, 0, 100])
kb_0 = np.array([0, 0, -100])

k_0 = np.array([kl_0, kr_0, kb_0, kt_0])

initial_conditions = np.concatenate([q_ic, q_dot_ic])

ans = solve_ivp(f, t_span, initial_conditions, args=(g, m, k, l_0), t_eval=t_eval, method='Radau')

np.savetxt("results.csv", ans.y.T, delimiter=",")
# print(sol)
fig = plt.figure()
# plt.plot(sol.t, sol.y[0], label='x')
# Create a 3D axis
ax = fig.add_subplot(111, projection='3d')

# Set labels
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_xlim([-60, 60])
ax.set_ylim([-60, 60])
ax.set_zlim([-60, 60])


data = ans.y.T
x_t = data[:, 0]
y_t = data[:, 1]
z_t = data[:, 2]

alpha_t = data[:, 3]
beta_t = data[:, 4]
gamma_t = data[:, 5]

v_x_t = data[:, 6]
v_y_t = data[:, 7]
v_z_t = data[:, 8]

v_alpha_t = data[:, 9]
v_beta_t = data[:, 10]
v_gamma_t = data[:, 11]


def getNormalVector(vectors, rotation_matrix, P_0):
    n=rotation_matrix @ np.cross(vectors[0] + P_0, vectors[1] + P_0)
    return n

point, = ax.plot(x_t[0], y_t[0], z_t[0], 'o')
rotation_angles_t = [alpha_t, beta_t, gamma_t]

q_t=[x_t, y_t, z_t, alpha_t, beta_t, gamma_t]
q_dot_t=[v_x_t, v_y_t, v_z_t, v_alpha_t, v_beta_t, v_gamma_t]
results = [q_t, q_dot_t]

def update(num, res, point):
    x_t, y_t, z_t, alpha_t, beta_t, gamma_t = results[0]
    v_x_t, v_y_t, v_z_t, v_alpha_t, v_beta_t, v_gamma_t = results[1]
    ax.clear()
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_xlim([-60, 60])
    ax.set_ylim([-60, 60])
    ax.set_zlim([-60, 60])
    P_0 = [x_t[num], y_t[num], z_t[num]]
    point.set_data(list([x_t[num], y_t[num]]))
    point.set_3d_properties(data[num, 2])
    rotation = [alpha_t[num], beta_t[num], gamma_t[num]]
    R = cp.getRotationMatrix(rotation)
    calc_vector_kr = np.array([1, 0, 0])*cube_k_vector_length
    calc_vector_kt = np.array([0, 0, 1])*cube_k_vector_length
    cube_k_vectors = np.array([calc_vector_kr, calc_vector_kt])
    cp.plotCubePosition(ax, cube_k_vectors, P_0, R, k_0)

    if hasattr(update, 'txt'):
        update.txt.remove()
    
    update.txt = ax.text(0, 0, 0, f'x={x_t[num]:.2f}, y={y_t[num]:.2f}, z={z_t[num]:.2f}, vx={v_x_t[num]:.2f}, vy={v_y_t[num]:.2f}, vz={v_z_t[num]:.2f}', color='black')


ani = animation.FuncAnimation(fig, update, frames=1000, fargs=(results, point))

# Save as gif
ani.save('trajectory.mp4', writer='ffmpeg', fps=1000/20)

plt.show()
