import pyg4ometry.visualisation as vis
import pyg4ometry.geant4 as geant4
import pyg4ometry.stl as stl
import pyg4ometry.gdml as gdml
import numpy as np

print("INFO: building geometry")

reg = geant4.Registry()

# NOTE: match sizes in MUGGenerator.cc (unit: m)
sky_height = 170
world_side = 330
ground_depth = 50
eps = 0.1 # gap between volumes and world

world_height = sky_height + ground_depth

# world volume (air)
world_s = geant4.solid.Box('WorldAir', world_side, world_side, world_height, reg, lunit='m')
world_l = geant4.LogicalVolume(world_s, 'G4_AIR', 'WorldAir', reg)
reg.setWorld(world_l)

# ground
ground_s = geant4.solid.Box('Ground', world_side-eps, world_side-eps, ground_depth-eps, reg, lunit='m')

l = 230./2
h = 139./2
pyramid_s = geant4.solid.GenericTrap('Pyramid',
        -l, -l, -l, l, l, l, l, -l,
        -0, -0, -0, 0, 0, 0, 0, -0,
        h,
        reg, lunit='m')

# adding an eps to make the ground and the pyramid overlap
concrete_s = geant4.solid.Union("Concrete", pyramid_s, ground_s,
        tra2=[[0, 0, 0], [0, 0, - (h + ground_depth/2) + eps, 'm']], registry=reg)

concrete_l = geant4.LogicalVolume(concrete_s, 'G4_CONCRETE', 'Concrete', reg)
geant4.PhysicalVolume([0, 0, 0], [0, 0, -world_height/2 + h + ground_depth - eps, 'm'],
        concrete_l, 'Concrete', world_l, reg)

det_s = geant4.solid.Box('Detector', 3, 3, 0.05, reg, 'm')
det_l = geant4.LogicalVolume(det_s, 'G4_PHOTO_EMULSION', 'Detector', reg)

queen_cham_floor = [0, 0, 0, 'm']
geant4.PhysicalVolume([0, 0, 0], queen_cham_floor, det_l,
        'LowerPanel', concrete_l, reg)

queen_cham_floor[2] += 1
geant4.PhysicalVolume([0, 0, 0], queen_cham_floor, det_l,
        'UpperPanel', concrete_l, reg)

print("INFO: exporting to geometry-simple.gdml")
w = gdml.Writer()
w.addDetector(reg)
w.write('geometry-simple.gdml')

# print("INFO: starting VTK GUI")
# v = vis.VtkViewer()
# v.addLogicalVolume(reg.getWorldVolume())
# v.addAxes(100000)
# v.setCameraFocusPosition()
# v.view()
