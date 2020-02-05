import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import numpy as np

data = np.loadtxt("data")

plt.imshow(data, norm=LogNorm())
plt.show()
