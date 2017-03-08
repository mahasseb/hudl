for f in $1/*.avi; do
./mos.sh $f
java -cp ./lib/hudl.jar edu/osu/hudl/mos/confidence/ConfidenceEstimator
rm -f prediction.txt
rm -f test
./klt.sh $f
java -cp ./lib/hudl.jar edu/osu/hudl/mos/klt/FusedMOSDetector tracks
cat final_output.txt >> mos_final_output.txt
done
