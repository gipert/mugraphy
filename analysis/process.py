import uproot
import awkward as ak
# import numpy as np
# import hist
import matplotlib
import matplotlib.pyplot as plt
import mplhep as hep
from hist import Hist

# hep.style.use(["fira", "firamath"])

with uproot.open('detector-hits.root:ntuples') as events:
    df = events.arrays(library='ak')

print('INFO: ', len(df), ' total events')

# select only rows with two-panel hits
sel = ak.num(df.panel, axis=1) == 2
df = df[sel]

print('INFO: ', len(df), ' events with two-panel hit')

# track direction - azimuthal angles
Dx = df.xhit[..., 0] - df.xhit[..., 1]
Dy = df.yhit[..., 0] - df.yhit[..., 1]
Dz = ak.mean(df.zhit[..., 0] - df.zhit[..., 1])

ang_x = Dx / Dz
ang_y = Dy / Dz

h1 = (
    Hist.new
    .Reg(100, -2, 2, name='x', label=r'$\tan\theta_x$')
    .Reg(100, -2, 2, name='y', label=r'$\tan\theta_y$')
    .Double()
)
h1.fill(x=ang_x, y=ang_y)

# h1.plot()
# h1.project("x").plot()
# h1.project("y").plot()
plt.figure(figsize=(8, 8))
h1.plot2d_full(top_ls='-', side_yerr=False, top_yerr=False)

theta = df.theta[..., 0]
h2 = Hist.new.Reg(100, 0, 180, name='x', label=r'Azimuthal angle $\theta$ (deg)').Double()
h2.fill(theta)
# h2.plot(yerr=False)

# deposited energy
he = Hist.new.Reg(100, 0, 100, name='x', label='Energy (GeV)').Double()
he.fill(ak.sum(df.energy, axis=-1))
# he.plot(ls='-', yerr=False)

plt.show()
