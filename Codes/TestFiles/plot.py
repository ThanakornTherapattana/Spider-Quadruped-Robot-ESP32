import matplotlib.pyplot as plt
import numpy as np

points = np.load('workspace.npy')

fig = plt.figure(figsize=(14, 5))

# Top view (X-Y plane) — lateral reach
ax1 = fig.add_subplot(131)
ax1.scatter(points[:,0], points[:,1], s=0.1, alpha=0.1, c='steelblue')
ax1.set_xlabel('X (mm)'); ax1.set_ylabel('Y (mm)')
ax1.set_title('Top view'); ax1.set_aspect('equal')

# Side view (X-Z plane) — height range
ax2 = fig.add_subplot(132)
ax2.scatter(points[:,0], points[:,2], s=0.1, alpha=0.1, c='coral')
ax2.set_xlabel('X (mm)'); ax2.set_ylabel('Z (mm)')
ax2.set_title('Side view'); ax2.set_aspect('equal')

# 3D view
ax3 = fig.add_subplot(133, projection='3d')
idx = np.random.choice(len(points), 5000)  # subsample for speed
ax3.scatter(points[idx,0], points[idx,1], points[idx,2], s=0.5, alpha=0.2)
ax3.set_xlabel('X'); ax3.set_ylabel('Y'); ax3.set_zlabel('Z')
ax3.set_title('3D workspace')


plt.tight_layout()
plt.show()