for f in tracks/*.txt; do java -cp .:./bin/:lib/iText-2.1.5.jar:lib/jcommon-1.0.18.jar:lib/jfreechart-1.0.15.jar:lib/jfreechart-1.0.15-demo.jar:lib/jfreechart-1.0.15-experimental.jar:lib/jfreechart-1.0.15-swt.jar:lib/JSAT_r413.jar:lib/junit.jar:lib/log4j-1.2.17.jar:lib/servlet.jar:lib/sgt_v30.jar:lib/swtgraphics2d.jar edu/osu/hudl/klt/KLTTrackVisualizer "$f"; done

