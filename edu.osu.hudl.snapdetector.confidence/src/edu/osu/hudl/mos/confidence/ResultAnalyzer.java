package edu.osu.hudl.mos.confidence;

import java.io.File;

import edu.osu.hudl.common.ClassificationType;
import edu.osu.hudl.common.VideoExample;
import edu.osu.hudl.common.VideoExampleLoader;

public class ResultAnalyzer {

	public static void main(String[] args) {
		VideoExampleLoader videoExampleLoader = new VideoExampleLoader();
		videoExampleLoader.loadSamples(ClassificationType.class1,
				"profile.log", "final_mapping.txt");
		int errorCount = 0;
		int earlyMOS = 0;
		for (int i = 0; i < videoExampleLoader.totalSamples.size(); i++) {
			VideoExample videoExample = (VideoExample) videoExampleLoader.totalSamples
					.get(i);
			int error = 15;
			int winsize = 50;
			if (videoExample.annotatedMomentOfSnap < winsize
					&& videoExample.annotatedMomentOfSnap > 0) {
				earlyMOS++;
			}
			if (Math.abs(videoExample.annotatedMomentOfSnap
					- videoExample.detectedMomentOfSnap) > error
					&& videoExample.annotatedMomentOfSnap < winsize
					&& videoExample.annotatedMomentOfSnap > 0) {
//				System.out.println("Video Name:"
//						+ videoExample.name
//						+ " AMOS:"
//						+ videoExample.annotatedMomentOfSnap
//						+ " DMOS:"
//						+ videoExample.detectedMomentOfSnap
//						+ " Error:"
//						+ (Math.abs(videoExample.annotatedMomentOfSnap
//								- videoExample.detectedMomentOfSnap)));
				System.out.println("videos/"+videoExample.name+".avi");
				errorCount++;
				File file = new File("videos" + File.separator
						+ videoExample.name + ".avi");
				file.renameTo(new File("chosen" + File.separator
						+ videoExample.name + ".avi"));
			}
		}

		System.out.println("----------------------------------------");
		System.out.println("early MOS:" + earlyMOS);
		System.out.println("Count:" + errorCount);
	}
}
