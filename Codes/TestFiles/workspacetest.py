import numpy as np
import pickle

L1, L2, L3 = 41.5, 95, 150

# Your joint limits (tune these to your physical robot)
t1_lim = np.radians([-60, 60])    # lateral swing
t2_lim = np.radians([-90, 45])    # shoulder up/down
t3_lim = np.radians([-120, 10])   # elbow

N = 100000
points = []

for _ in range(N):
    t1 = np.random.uniform(*t1_lim)
    t2 = np.random.uniform(*t2_lim)
    t3 = np.random.uniform(*t3_lim)

    # FK using your DH parameters
    x = (L1 + L2*np.cos(t2) + L3*np.cos(t2+t3)) * np.cos(t1)
    y = (L1 + L2*np.cos(t2) + L3*np.cos(t2+t3)) * np.sin(t1)
    z =       L2*np.sin(t2) + L3*np.sin(t2+t3)

    points.append([x, y, z])

points = np.array(points)
np.save('workspace.npy', points)   # save for later use