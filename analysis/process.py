import sys

# import hist
# import matplotlib
import matplotlib.pyplot as plt

# import awkward as ak
import numpy as np
import uproot

# import mplhep as hep
from hist import Hist

# hep.style.use(["fira", "firamath"])

with uproot.open(sys.argv[1] + ":ntuples") as events:
    df = events.arrays(library="ak", filter_name=["theta", "phi"])

# df = uproot.concatenate("detector-hits*.root:ntuples")

print("INFO: ", len(df), " total events")

theta = df.theta[..., 0]
phi = df.phi[..., 0]

ang_x = np.tan(theta) * np.cos(phi)
ang_y = np.tan(theta) * np.sin(phi)

h1 = (
    Hist.new.Reg(100, -1, 1, name="x", label=r"$\tan\theta_x$")
    .Reg(100, -1, 1, name="y", label=r"$\tan\theta_y$")
    .Double()
)
h1.fill(x=ang_x, y=ang_y)

plt.figure(figsize=(9, 8))
h1.plot()
# h1.project("x").plot()
# h1.project("y").plot()
# plt.figure(figsize=(8, 8))
# h1.plot2d_full(top_ls='-', side_yerr=False, top_yerr=False)
# h1.plot2d_full(top_ls='-', side_yerr=False, top_yerr=False, main_cmap='rainbow')

# h2 = Hist.new.Reg(
#     100, 0, np.pi, name="x", label=r"Azimuthal angle $\theta$ (deg)"
# ).Double()
# h2.fill(theta)
# h2.plot(yerr=False)

# deposited energy
# he = Hist.new.Reg(100, 0, 100, name='x', label='Energy (GeV)').Double()
# he.fill(df.energy[..., 0])
# he.plot(yerr=False)

plt.show()
