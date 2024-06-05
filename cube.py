import sympy as sp
import numpy as np
from scipy.integrate import solve_ivp
from scipy.integrate import odeint
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
from sympy import pprint


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

# Initial displacement vectors for the four springs
vector_kr = sp.Matrix([30, 0, 0])
vector_kl = sp.Matrix([-30, 0, 0])
vector_kt = sp.Matrix([0, 0, 30])
vector_kb = sp.Matrix([0, 0, -30])

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

t_span = [0, 5]
t_eval = np.linspace(*t_span, 1000)

q_ic = np.array([-40, -40, 20, 5, 0, 0])
q_dot_ic = np.array([0, 0, 0, 0, 0, 0])
g = 9.81
m = 20
k = 2
l_0 = 40

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
ax.set_xlim([-100, 100])
ax.set_ylim([-100, 100])
ax.set_zlim([-100, 100])

# ax.plot(ans.y[:, 0], ans.y[:, 1], ans.y[:, 2])

point, = ax.plot([ans.y[0, 0]], [ans.y[0, 1]], [ans.y[0, 2]], 'o')

# Update function for animation
def update(num, ans, point):
    point.set_data(list(ans.y[num, 0:2]))
    point.set_3d_properties(ans.y[num, 2])
    
    # Remove previous text
    if hasattr(update, 'txt'):
        update.txt.remove()
    
    # Add text
    update.txt = ax.text(0, 0, 0, f'x={ans.y[num, 0]:.2f}, y={ans.y[num, 1]:.2f}, z={ans.y[num, 2]:.2f}, vx={ans.y[num, 6]:.2f}, vy={ans.y[num, 7]:.2f}, vz={ans.y[num, 8]:.2f}', color='black')

fps = ans.y.shape[0] / (t_span[1] - t_span[0])

ani = animation.FuncAnimation(fig, update, frames=ans.y.shape[0], fargs=(ans, point))

# Save as gif
ani.save('trajectory.mp4', writer='ffmpeg', fps=fps)
ani.save('trajectory.gif', writer='pillow', fps=fps)
plt.show()
