from pyg4ometry import geant4
from pyg4ometry import stl
from pyg4ometry import gdml
from pyg4ometry import visualisation as vis
import numpy as np

# resources:
# - https://www.nature.com/articles/nature24647
# - https://en.wikipedia.org/wiki/Great_Pyramid_of_Giza
# - https://arxiv.org/abs/2202.07434

enable_cavities = True
enable_pyramid = True

print("INFO: building geometry")

reg = geant4.Registry()

# NOTE: match sizes in MUGGenerator.cc (unit: m)
sky_height = 170
world_side = 330
ground_depth = 50
eps = 0.001  # gap between volumes and world

world_height = sky_height + ground_depth

# world volume (air)
world_s = geant4.solid.Box('WorldAir', world_side, world_side, world_height, reg, lunit='m')
world_l = geant4.LogicalVolume(world_s, 'G4_AIR', 'WorldAir', reg)
reg.setWorld(world_l)

# generic limestone material
element = geant4.nist_element_2geant4Element
rock_mat = geant4.MaterialCompound("Limestone", 2.2, 3, registry=reg)
rock_mat.add_element_massfraction(element('G4_C'),  0.12)
rock_mat.add_element_massfraction(element('G4_O'),  0.48)
rock_mat.add_element_massfraction(element('G4_Ca'), 0.4)

# ground
ground_s = geant4.solid.Box('Ground', world_side-eps, world_side-eps, ground_depth-eps, reg, lunit='m')
ground_l = geant4.LogicalVolume(ground_s, rock_mat, 'Ground', reg)
geant4.PhysicalVolume([0, 0, 0], [0, 0, -world_height/2 + ground_depth/2 + eps/2, 'm'],
                      ground_l, 'Ground', world_l, reg)

ground_level = -world_height/2 + ground_depth
print('INFO: ground level is at z =', -world_height/2 + ground_depth, 'm')

# pyramid
p_side = 230.3
p_height = 138.5
p_height_orig = 146.5
dl = (p_height_orig - p_height) * p_side/2/p_height
pyramid_s = geant4.solid.Trd('Pyramid', p_side, dl, p_side, dl, p_height, reg, lunit='m')

pyramid_l = geant4.LogicalVolume(pyramid_s, rock_mat, 'Pyramid', reg)
if enable_pyramid:
    geant4.PhysicalVolume([0, 0, 0], [0, 0, -world_height/2 + ground_depth + p_height/2 + eps, 'm'],
                          pyramid_l, 'Pyramid', world_l, reg)

p_off = -p_height/2

if enable_cavities:
    # Queen's chamber core
    queen_cham_s = geant4.solid.Box('QueenChamber', 5.3, 6.7, 4.7, reg, lunit='m')
    queen_cham_l = geant4.LogicalVolume(queen_cham_s, 'G4_AIR', 'QueenChamber', reg)
    geant4.PhysicalVolume([0, 0, 0], [0, 1.8, 23.3 + p_off, 'm'],
                          queen_cham_l, 'QueenChamber', pyramid_l, reg)
    # Queen's chamber hat
    queen_hat_s = geant4.solid.Trd('QueenHat', 5.3, 0, 6.7, 6.7, 1.5, reg, lunit='m')
    queen_hat_l = geant4.LogicalVolume(queen_hat_s, 'G4_AIR', 'QueenHat', reg)
    geant4.PhysicalVolume([0, 0, 0], [0, 1.8, 23.3 + 4.7/2 + 1.5/2 + eps + p_off, 'm'],
                          queen_hat_l, 'QueenHat', pyramid_l, reg)

    # King's chamber
    king_cham_dx = 5.2
    king_cham_dy = 10.6
    king_cham_dz = 5.7
    king_cham_s = geant4.solid.Box('KingChamber', king_cham_dx, king_cham_dy, king_cham_dz,
                                   reg, lunit='m')
    king_cham_l = geant4.LogicalVolume(king_cham_s, 'G4_AIR', 'KingChamber', reg)
    geant4.PhysicalVolume([0, 0, 0], [-10.9, 4.7, 42.8 + p_off, 'm'],
                          king_cham_l, 'KingChamber', pyramid_l, reg)

    king_buff_dz = [0.88, 1.02, 1.33, 1.24]
    king_buff_z = [2.49, 5.13, 7.95, 10.66]  # relative to the roof of King's chamber
    for i in range(len(king_buff_z)):
        king_buff_s = geant4.solid.Box('KingChamberAirBuffer'+str(i), king_cham_dx, king_cham_dy, king_buff_dz[i],
                                       reg, lunit='m')
        king_buff_l = geant4.LogicalVolume(king_buff_s, 'G4_AIR', 'KingChamberAirBuffer'+str(i), reg)
        geant4.PhysicalVolume([0, 0, 0], [-10.9, 4.7, 42.8 + king_cham_dz/2 + king_buff_z[i] + p_off, 'm'],
                              king_buff_l, 'KingChamberAirBuffer'+str(i), pyramid_l, reg)

    # King's chamber hat
    king_hat_s = geant4.solid.Trd('KingHat', 6.2, 0, king_cham_dy, king_cham_dy, 2.6, reg, lunit='m')
    king_hat_l = geant4.LogicalVolume(king_hat_s, 'G4_AIR', 'KingHat', reg)
    geant4.PhysicalVolume([0, 0, 0], [-10.9, 4.7, 42.8 + king_cham_dz/2 + 13.9 + p_off, 'm'],
                          king_hat_l, 'KingHat', pyramid_l, reg)

    # Shaft from Grand gallery to King's chamber 2.74
    king_shaft_s = geant4.solid.Box('KingShaft', 6.9, 1.1, 1.1, reg, lunit='m')
    king_shaft_l = geant4.LogicalVolume(king_shaft_s, 'G4_AIR', 'KingShaft', reg)
    geant4.PhysicalVolume([0, 0, 0], [-4.8, 0, 40.5 + p_off, 'm'],
                          king_shaft_l, 'KingShaft', pyramid_l, reg)

    king_shaft2_s = geant4.solid.Box('KingShaft2', 2.9, 1.1, 2.74, reg, lunit='m')
    king_shaft2_l = geant4.LogicalVolume(king_shaft2_s, 'G4_AIR', 'KingShaft2', reg)
    geant4.PhysicalVolume([0, 0, 0], [-4.0, 0, 40.5 + 1.92 + eps + p_off, 'm'],
                          king_shaft2_l, 'KingShaft2', pyramid_l, reg)

    # Grand gallery
    grand_gallery_slope = 26.7
    grand_gallery_s = geant4.solid.Para('GrandGallery', 47.2, 2.1, 7.7,
                                        0, -grand_gallery_slope*np.pi/180, 0,
                                        reg, lunit='m')
    grand_gallery_l = geant4.LogicalVolume(grand_gallery_s, 'G4_AIR', 'GrandGallery', reg)
    geant4.PhysicalVolume([0, -grand_gallery_slope*np.pi/180, 0], [19.8, 0, 33.6 + p_off, 'm'],
                          grand_gallery_l, 'GrandGallery', pyramid_l, reg)


# detector(s)
det_s = geant4.solid.Box('Detector', 5.3-eps, 6.7-eps, 0.05, reg, 'm')
det_l = geant4.LogicalVolume(det_s, 'G4_PHOTO_EMULSION', 'Detector', reg)

if enable_cavities:
    queen_cham_floor = [0, 0, -2.375 + 0.1, 'm']  # why do I need to add this much??
    mother = queen_cham_l
elif enable_pyramid:
    queen_cham_floor = [0, 1.3, 20.95 + p_off, 'm']
    mother = pyramid_l
else:
    queen_cham_floor = [0, 1.3, ground_level + 20.95, 'm']
    mother = world_l

geant4.PhysicalVolume([0, 0, 0], queen_cham_floor, det_l,
                      'LowerPanel', mother, reg, copyNumber=0)

# queen_cham_floor[2] += 1
# geant4.PhysicalVolume([0, 0, 0], queen_cham_floor, det_l,
#                       'UpperPanel', mother, reg, copyNumber=1)

world_l.checkOverlaps(recursive=True)

print("INFO: exporting to geometry.gdml")
w = gdml.Writer()
w.addDetector(reg)
w.write('geometry.gdml')

# print("INFO: starting VTK GUI")
# v = vis.VtkViewer()
# v.addLogicalVolume(reg.getWorldVolume())
# v.addAxes(100000)
# v.setCameraFocusPosition(focalPoint=[0,0,0], position=[1,0,0])
# v.view()
