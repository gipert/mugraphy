import pyg4ometry.visualisation as vis
import pyg4ometry.geant4 as geant4
import pyg4ometry.freecad as freecad
import pyg4ometry.gdml as gdml
import numpy as np

reg = geant4.Registry()

# match sizes in MUGGenerator.cc
world_s = geant4.solid.Box('WorldAir', 330, 330, 190, reg, 'm')
world_l = geant4.LogicalVolume(world_s, 'G4_AIR', 'WorldAir', reg)
reg.setWorld(world_l)

r = freecad.Reader('khufu-pyramid.step')
r.relabelModel()
r.convertFlat(daughterMaterial='G4_CONCRETE')
pyramid_l = r.getRegistry().getWorldVolume()

# TODO: place the pyramid at the correct height
pyramid_s = geant4.PhysicalVolume([-np.pi/2, 0, 0], [0, 0, 0],
        pyramid_l.assemblyVolume(), 'Pyramid', world_l, reg)

reg.addVolumeRecursive(pyramid_s)

# ?
queen_cham_l = reg.logicalVolumeDict['Shell140_lv']

# det_s = geant4.solid.Box('Detector', 2, 2, 0.05, reg, 'm')
# det_l = geant4.LogicalVolume(det_s, 'G4_PLASTIC_SC_VINYLTOLUENE', 'Detector', reg)
# det_s = geant4.PhysicalVolume([0, 0, 0], [0, 0, 0], det_l, 'Detector', queen_cham_l, reg)

w = gdml.Writer()
w.addDetector(reg)
w.write('geometry.gdml')

v = vis.VtkViewer()
v.addLogicalVolume(reg.getWorldVolume())
# v.addLogicalVolume(queen_cham_l)
v.addAxes(10000)
v.setCameraFocusPosition()
v.view()
