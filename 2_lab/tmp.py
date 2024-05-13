import numpy as np

# Given atomic radius for iron in meters (for a BCC lattice)
R_iron = 0.124e-9  # convert nm to m
lambda_xray_bcc = 0.0711e-9  # Wavelength of the X-ray in meters

# Calculate the edge length 'a' of the BCC unit cell
a_BCC = 2* np.sqrt(2) * R_iron

# Miller indices for the (310) plane
h, k, l = 3, 1, 0

# Calculate the distance 'd' between the (310) planes in the BCC lattice
d_BCC_310 = a_BCC / np.sqrt(h**2 + k**2 + l**2)

# Bragg's Law to find the angle theta (for n=1)
theta_bcc = np.arcsin(lambda_xray_bcc / (2 * d_BCC_310))

# The diffraction angle 2θ for BCC lattice
two_theta_bcc = 2 * theta_bcc * (180/np.pi)  # Convert the angle to degrees
print(two_theta_bcc)
