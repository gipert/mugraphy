from pyg4ometry import geant4
from pyg4ometry import stl
from pyg4ometry import gdml
from pyg4ometry import visualisation as vis
import numpy as np

enable_cavities = False

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

# rock_mat = geant4.MaterialPredefined('G4_CONCRETE')
element = geant4.nist_element_2geant4Element
rock_mat = geant4.MaterialCompound("Limestone", 10, 10, registry=reg)
rock_mat.add_element_massfraction(element('G4_H'),  0.01)
rock_mat.add_element_massfraction(element('G4_C'),  0.001)
rock_mat.add_element_massfraction(element('G4_O'),  0.529107)
rock_mat.add_element_massfraction(element('G4_Na'), 0.016)
rock_mat.add_element_massfraction(element('G4_Mg'), 0.002)
rock_mat.add_element_massfraction(element('G4_Al'), 0.033872)
rock_mat.add_element_massfraction(element('G4_Si'), 0.337021)
rock_mat.add_element_massfraction(element('G4_K'),  0.013)
rock_mat.add_element_massfraction(element('G4_Ca'), 0.044)
rock_mat.add_element_massfraction(element('G4_Fe'), 0.014)

concrete_l = geant4.LogicalVolume(concrete_s, rock_mat, 'Concrete', reg)
geant4.PhysicalVolume([0, 0, 0], [0, 0, -world_height/2 + h + ground_depth - eps, 'm'],
        concrete_l, 'Concrete', world_l, reg)

if enable_cavities:
    # read CAD model of cavities in
    r = stl.Reader("khufu-pyramid.stl", solidname="Cavities", scale=1000, registry=reg)
    cavities_s = r.getSolid()
    cavities_l = geant4.LogicalVolume(cavities_s, "G4_AIR", "Cavities", reg)
    geant4.PhysicalVolume([0, 0, 0], [0, 19.5, -75, 'm'],
            cavities_l, 'Cavities', concrete_l, reg)

det_s = geant4.solid.Box('Detector', 3, 3, 0.05, reg, 'm')
det_l = geant4.LogicalVolume(det_s, 'G4_PHOTO_EMULSION', 'Detector', reg)

queen_cham_floor = [0, -22, 18, 'm']
geant4.PhysicalVolume([0, 0, 0], queen_cham_floor, det_l,
        'LowerPanel', cavities_l if enable_cavities else concrete_l,
        reg, copyNumber=0)

queen_cham_floor[2] += 1
geant4.PhysicalVolume([0, 0, 0], queen_cham_floor, det_l,
        'UpperPanel', cavities_l if enable_cavities else concrete_l,
        reg, copyNumber=1)

# cavities_l.checkOverlaps()

print("INFO: exporting to geometry.gdml")
w = gdml.Writer()
w.addDetector(reg)
w.write('geometry.gdml')

# print("INFO: starting VTK GUI")
# v = vis.VtkViewer()
# v.addLogicalVolume(reg.getWorldVolume())
# v.addAxes(100000)
# v.setCameraFocusPosition()
# v.view()
