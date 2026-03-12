#!/bin/bash
#SBATCH --job-name=pddgrid
#SBATCH --output=logs/pddgrid_%A_%a.out
#SBATCH --error=logs/pddgrid_%A_%a.err
#SBATCH --time=10:00:00
#SBATCH --cpus-per-task=13
#SBATCH --mem=8G
#SBATCH --array=1-200%1
#SBATCH --requeue

set -euo pipefail

module purge
module load Geant4/11.3.0-GCC-13.3.0
module load Geant4-data/11.3

cd /scratch/brussel/113/vsc11383/projects/third_sim/FlashElectronSim/build
export G4FORCENUMBEROFTHREADS=${SLURM_CPUS_PER_TASK}

CSV="../macros/3paramgrid.csv"

# Grab Nth data row (skip header)
LINE=$(awk -F',' -v n="${SLURM_ARRAY_TASK_ID}" 'NR==n+1 {print $0}' "${CSV}")
if [[ -z "${LINE}" ]]; then
  echo "No row for task ${SLURM_ARRAY_TASK_ID} in ${CSV}"
  exit 1
fi

# CSV columns:
               # run_id,beamOn,Emean_MeV,sigmaE_MeV,sigmaTheta_deg,sigma_x_mm,sigma_y_mm
IFS=',' read -r RUNID BEAMON EMEAN SIGE SIGTH SX SY <<< "${LINE}"

echo "Task ${SLURM_ARRAY_TASK_ID}: run_id=${RUNID} beamOn=${BEAMON} E=${EMEAN} sigE=${SIGE} sigTh=${SIGTH} SX=${SX}"

# Make a short tag for filenames
TAG="r${RUNID}_E${EMEAN}_sE${SIGE}_sTh${SIGTH}"

# Helper: replace '.' with 'p' to keep filenames clean
TAG_CLEAN=$(echo "${TAG}" | sed 's/\./p/g')

# ---- generate source macro ----
SRCMAC="src_${TAG_CLEAN}_J${SLURM_JOB_ID}_T${SLURM_ARRAY_TASK_ID}.mac"

sed -e "s/EMEAN/${EMEAN}/g" \
    -e "s/SIGE/${SIGE}/g" \
    -e "s/SIGTH/${SIGTH}/g" \
    -e "s/SX/${SX}/g" \
    -e "s/SY/${SY}/g" \
    ../macros/sourcepossib.mac > "${SRCMAC}"

# Output files (3 of them)
PDDTXT="pdd_${TAG_CLEAN}_N${BEAMON}_J${SLURM_JOB_ID}_T${SLURM_ARRAY_TASK_ID}.txt"
EXITDOSE="exitDose_${TAG_CLEAN}_N${BEAMON}_J${SLURM_JOB_ID}_T${SLURM_ARRAY_TASK_ID}.txt"
EXITFLU="exitFlu_${TAG_CLEAN}_N${BEAMON}_J${SLURM_JOB_ID}_T${SLURM_ARRAY_TASK_ID}.txt"

RUNMAC="$(mktemp run_${TAG_CLEAN}_XXXX.mac)"
trap 'rm -f "$RUNMAC" "$SRCMAC"' EXIT

# Replace placeholders in run template
sed -e "s|SOURCE_MAC|${SRCMAC}|g" \
    -e "s/BEAMON/${BEAMON}/g" \
    -e "s|PDD_TXT|${PDDTXT}|g" \
    -e "s|EXIT_DOSE_TXT|${EXITDOSE}|g" \
    -e "s|EXIT_FLUENCE_TXT|${EXITFLU}|g" \
    ../macros/pddpossib.mac > "${RUNMAC}"

./FlashElectronSim "${RUNMAC}"

echo "Wrote: ${PDDTXT}"
echo "Wrote: ${EXITDOSE}"
echo "Wrote: ${EXITFLU}"
