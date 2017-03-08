package edu.osu.hudl.mos.klt;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Vector;

import edu.osu.hudl.common.IExample;
import edu.osu.hudl.common.MOSOutputLoader;
import edu.osu.hudl.common.VideoExample;

public class FusedMOSDetector {
	public static void main(String[] args) {
		Vector<IExample> examples = MOSOutputLoader
				.load(".confidence_output.txt");
		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File("final_output.txt")));
			for (IExample iExample : examples) {
				VideoExample videoExample = (VideoExample) iExample;
				int mos = -1;
				int maxDifference = Integer.MIN_VALUE;
				if (videoExample.getConfidence() != 1) {
					System.out
							.println("------------------------------------------------------------");
					System.out.println("Finding MOS using KLT track for video:"
							+ videoExample.name);
					Video video = null;
					try {
						video = KLTTrackLoader.loadTracks(args[0]
								+ File.separator + "tracks_"
								+ videoExample.name + ".txt");
						Vector<Track> tracks = video.tracks;
						int margin = 15;
						Vector<Integer> trackCounts = new Vector<Integer>();
						for (int frameIndex = margin; frameIndex < 500; frameIndex++) {
							int count = 0;
							for (Track track : tracks) {
								if (track.startFrame >= frameIndex - margin
										&& track.endFrame <= frameIndex
										&& track.length < 10) {
									count++;
								}
							}
							trackCounts.add(count);
						}
						for (int i = 0; i < trackCounts.size() - margin; i++) {
							if (maxDifference < trackCounts.get(i + margin)
									- trackCounts.get(i)
									&& trackCounts.get(i) < 250) {
								maxDifference = trackCounts.get(i + margin)
										- trackCounts.get(i);
								mos = i + margin;
							}
						}
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
						mos = videoExample.detectedMomentOfSnap;
					}
				} else {
					System.out
							.println("------------------------------------------------------------");
					System.out
							.println("Refining MOS using KLT track for video:"
									+ videoExample.name);
					// Video video = null;
					try {
						// video = KLTTrackLoader.loadTracks(args[0]
						// + File.separator + "tracks_"
						// + videoExample.name + ".txt");
						// Vector<Track> tracks = video.tracks;
						//
						// int margin = 15;
						// Vector<Integer> trackCounts = new Vector<Integer>();
						// for (int frameIndex = margin; frameIndex < 500;
						// frameIndex++) {
						// int count = 0;
						// for (Track track : tracks) {
						// if (track.startFrame >= frameIndex - margin
						// && track.endFrame <= frameIndex
						// && track.length < 10) {
						// count++;
						// }
						// }
						// trackCounts.add(count);
						// }
						// int oldMOS = videoExample.detectedMomentOfSnap;
						// for (int frame = oldMOS - margin; frame < oldMOS
						// + margin; frame++) {
						// for (int offset = -margin; offset < margin; offset++)
						// {
						// if (maxDifference < trackCounts.get(frame)
						// - trackCounts.get(frame + offset)) {
						// maxDifference = trackCounts.get(frame)
						// - trackCounts.get(frame + offset);
						// mos = frame;
						// }
						// }
						// }
						mos = videoExample.detectedMomentOfSnap;
					} catch (Exception e) {
						mos = videoExample.detectedMomentOfSnap;
					}
				}
				bufferedWriter.write("Video Name : " + videoExample.name
						+ " ;Moment of Snap:" + mos + " ;Confidence: "
						+ videoExample.getConfidence() + "\n");
				bufferedWriter.flush();
			}
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
