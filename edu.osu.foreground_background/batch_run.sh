HOMOS=(game02_homo/002.txt game02_homo/003.txt game02_homo/004.txt game02_homo/006.txt game02_homo/012.txt game02_homo/040.txt game02_homo/072.txt game02_homo/088.txt)
FILES=(Game02/Game02_video0002.avi Game02/Game02_video0003.avi Game02/Game02_video0004.avi Game02/Game02_video0006.avi Game02/Game02_video0012.avi Game02/Game02_video0040.avi Game02/Game02_video0072.avi Game02/Game02_video0088.avi) 
OUTPUTS=(Game02/Game02_video0002 Game02/Game02_video0003 Game02/Game02_video0004 Game02/Game02_video0006 Game02/Game02_video0012 Game02/Game02_video0040 Game02/Game02_video0072 Game02/Game02_video0088)
i=0;
j=1;
for f in $FILES
do
  echo "Processing $f file..." 
  env LD_LIBRARY_PATH=:/scratch/installed/lib:/scratch/software/caffe-master/build/lib/:/usr/lib64/atlas/:/opt/cuda-6-0/lib64/:/scratch/installed/lib:/scratch/git/bm/bm-common/bin/:/scratch/git/bm/bm-vision/bin/:/scratch/git/bm/bm-deeplearning/bin/:/scratch/software/darwin.1.7/bin/ ./bin/hudl $f ${HOMOS[i]} game02.png 1 2 0.005 ${OUTPUTS[i]} 
  let i=i+1
done

