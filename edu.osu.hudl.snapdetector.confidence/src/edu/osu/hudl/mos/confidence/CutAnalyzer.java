package edu.osu.hudl.mos.confidence;

import edu.osu.hudl.common.ClassificationType;
import edu.osu.hudl.common.VideoExampleLoader;

public class CutAnalyzer {
	public static void main(String[] args) {
		VideoExampleLoader videoExampleLoader = new VideoExampleLoader();
		videoExampleLoader.loadSamples(ClassificationType.class1,
				"profile.log", "final_mapping.txt");

	}
}
