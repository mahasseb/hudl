./mos.sh $1
java -cp ./lib/hudl.jar edu/osu/hudl/mos/confidence/ConfidenceEstimator
rm -f prediction.txt
rm -f test
