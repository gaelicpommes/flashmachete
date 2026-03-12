#!/bin/bash
#SBATCH --job-name=pdd
#SBATCH --output=logs/pdd_%A_%a.out
#SBATCH --error=logs/pdd_%A_%a.err
#SBATCH --time=10:00:00
#SBATCH --cpus-per-task=12
#SBATCH --mem=8G
#SBATCH --array=1-1%1
#SBATCH --requeue
set -euo pipefail

module purge
module load Geant4/11.3.0-GCC-13.3.0
module load Geant4-data/11.3

cd /scratch/brussel/113/vsc11383/projects/third_sim/FlashElectronSim/build
export G4FORCENUMBEROFTHREADS=${SLURM_CPUS_PER_TASK}

case "${SLURM_ARRAY_TASK_ID}" in
  1) N=900000 ;;
  2) N=1 ;;
  3) N=1;;
  *) echo "Invalid SLURM_ARRAY_TASK_ID=${SLURM_ARRAY_TASK_ID}" ; exit 1 ;;
esac

# Pretty label: 5000->5k, 50000->50k, 100000->100k
if   [ "$N" -ge 1000000 ]; then LABEL="$((N/1000000))M"
elif [ "$N" -ge 1000    ]; then LABEL="$((N/1000))k"
else LABEL="${N}"
fi

PDD_TXT="pdd_${LABEL}_J${SLURM_ARRAY_JOB_ID}_T${SLURM_ARRAY_TASK_ID}.txt"
EDOSE_TXT="exitDose_${LABEL}_J${SLURM_ARRAY_JOB_ID}_T${SLURM_ARRAY_TASK_ID}.txt"
EFLU_TXT="exitFluence_${LABEL}_J${SLURM_ARRAY_JOB_ID}_T${SLURM_ARRAY_TASK_ID}.txt"

TMPMAC="$(mktemp beamdiag_${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}_XXXX.mac)"
trap 'rm -f "$TMPMAC"' EXIT

sed -e "s/BEAMON/${N}/g" \
    -e "s|PDD_TXT|${PDD_TXT}|g" \
    -e "s|EXIT_DOSE_TXT|${EDOSE_TXT}|g" \
    -e "s|EXIT_FLUENCE_TXT|${EFLU_TXT}|g" \
    ../macros/beamdiag_template.mac > "$TMPMAC"

./FlashElectronSim "$TMPMAC"

echo "Wrote: $PDD_TXT"
echo "Wrote: $EDOSE_TXT"
echo "Wrote: $EFLU_TXT"
