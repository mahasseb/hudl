KLT_PATH=../edu.osu.vision.klt-tracker/
env LD_LIBRARY_PATH=/usr/local/lib/:/home/behrooz/workspace/edu.osu.vision.common/ $KLT_PATH/klt $1
rm -f features.ft
rm -f features.txt
