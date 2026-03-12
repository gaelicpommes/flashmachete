# Load toolchain + libs
module purge
module load foss/2023a
module load CMake/3.26.3-GCCcore-12.3.0
module load Qt5/5.15.10-GCCcore-12.3.0
module load Xerces-C++/3.2.4-GCCcore-12.3.0
module load ROOT/6.30.06-foss-2023a

# Paths
export PREFIX=$VSC_SCRATCH/gate_geant4

# Geant4 env
source /scratch/brussel/113/vsc11383/geant4/install/bin/geant4.sh

# GATE in PATH
export PATH=$PREFIX/install/gate/bin:$PATH

