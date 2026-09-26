import matplotlib.pyplot as plt
import numpy as np

t = np.linspace(0, 1, 10)
print(t)
step_len = 60   # mm
step_h   = 15   # mm

foot_x = np.zeros_like(t)   # straight forward, no lateral shift
foot_z = step_h  * (1 - np.cos(2*np.pi*t)) / 2
foot_y = step_len * (t - np.sin(2*np.pi*t) / (2*np.pi))

plt.figure(figsize=(14, 5))
# Overlay on side view
plt.plot(foot_y, foot_z, 'r-', linewidth=2, label='Foot path')
plt.legend()

plt.show()