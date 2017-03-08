for f in $1/*.avi; do
./mos.sh $f
java -cp ./lib/hudl.jar edu/osu/hudl/mos/confidence/ConfidenceEstimator
rm -f prediction.txt
rm -f test
cat  .confidence_output.txt >> mos_final_output.txt
cat .profile.log >> profile.log
done
