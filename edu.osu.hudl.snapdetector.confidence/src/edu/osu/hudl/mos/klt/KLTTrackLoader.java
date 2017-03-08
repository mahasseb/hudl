package edu.osu.hudl.mos.klt;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Vector;

public class KLTTrackLoader {
	public static Video loadTracks(String filename) throws Exception {
		 System.out.println("loadTracks-->" + filename);
		Video video = new Video();
		try {
			if (!new File(filename).exists())
				throw new Exception();
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File(filename)));
			String line = bufferedReader.readLine();
			// String[] strings = line.split(",");
			// for (String string : strings) {
			// video.localmaximums.add(Integer.valueOf(string.trim()));
			// }
			// line = bufferedReader.readLine();
			while (line != null) {
				if (!line.contains("e-") && !line.contains("e+")
						&& !line.contains("(-") && !line.contains(",-")
						&& !line.contains("0x") && line.startsWith("T")) {
					try {
						Track track = new Track();
						track.name = line.split(":")[0];
						String strings1[] = line.split(":")[1].trim()
								.split("-");
						track.startFrame = Integer.valueOf(strings1[0]
								.substring(1, strings1[0].length() - 1).split(
										",")[0]);
						track.endFrame = Integer.valueOf(strings1[0].substring(
								1, strings1[0].length() - 1).split(",")[1]);
						String points[] = strings1[1].split(";");

						int index = track.startFrame;
						for (String string : points) {
							Point point = new Point();
							point.x = Float.valueOf(string.substring(1,
									string.length() - 1).split(",")[0]);
							point.y = Float.valueOf(string.substring(1,
									string.length() - 1).split(",")[1]);
							point.frame = index;
							index++;
							track.points.add(point);
						}
						track.length = Math.sqrt(Math.pow(track.points.get(0).x
								- track.points.get(track.points.size() - 1).x,
								2)
								+ Math.pow(
										track.points.get(0).y
												- track.points.get(track.points
														.size() - 1).y, 2));
						video.tracks.add(track);
					} catch (Throwable throwable) {
						throwable.printStackTrace();
					}
				}
				line = bufferedReader.readLine();
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return video;
	}
}
