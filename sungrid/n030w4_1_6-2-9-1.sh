#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch optimal solver and then the validator

./bin/optimalRoster n030w4 1 6 2 9 1 n030w4_1_6-2-9-1 > outfiles/Competition/n030w4_1_6-2-9-1/log.txt

instance=n030w4
weeksValue=(6 2 9 1 )

demand0="WD-${instance}-"
solutionFile="outfiles/Competition/n030w4_1_6-2-9-1/OptSol-n030w4-6291-"
weeks=""
sols=""
i=0

for var in ${weeksValue[*]}
do
demand[$i]="datasets/${instance}/${demand0}${var}.txt"
weeks="${weeks} ${demand[$i]}"
solution[$i]="${solutionFile}${var}-${i}.txt"
sols="${sols} ${solution[$i]}"
((i++))
done

java -jar validator.jar --sce datasets/n030w4/Sc-n030w4.txt --his datasets/n030w4/H0-n030w4-1.txt --weeks $weeks --sols $sols > outfiles/Competition/n030w4_1_6-2-9-1/validatorOutput.txt 

exit 0;
