./clean.sh
LD_LIBRARY_PATH=./lib/:/usr/local/lib/
export LD_LIBRARY_PATH
./mosdetector "$1" .mos_output.txt -p .profile.log -w 100

