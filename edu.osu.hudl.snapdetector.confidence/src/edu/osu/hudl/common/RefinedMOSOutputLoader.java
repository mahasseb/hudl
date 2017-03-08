package edu.osu.hudl.common;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class RefinedMOSOutputLoader {
	public static Vector<IExample> load(String string) {
		Vector<IExample> examples = new Vector<IExample>();
		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File(string)));
			String line = bufferedReader.readLine();
			while (line != null) {
				String[] strings = line.split(";");
				VideoExample videoExample = new VideoExample();
				videoExample.name = strings[0].split(":")[1].trim();
				videoExample.detectedMomentOfSnap = Integer.valueOf(strings[1]
						.split(":")[1].trim());
				videoExample.confidence = Float
						.valueOf(strings[2].split(":")[1].trim());
				examples.add(videoExample);
				line = bufferedReader.readLine();
			}
			bufferedReader.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return examples;
	}

	public static Vector<IExample> load(String string, String mappingFile) {
		Map<String, Video> videosMap = new HashMap<String, Video>();
		try {

			// Reading mapping.txt
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File(mappingFile)));
			String line = bufferedReader.readLine();
			while (line != null) {
				String[] strings = line.split(";");
				if (line.trim().length() == 0)
					break;
				Video video = new Video();
				video.name = strings[1];
				video.annotatedMOS = Integer.valueOf(strings[2]);
				videosMap.put(video.name, video);
				line = bufferedReader.readLine();
			}
			bufferedReader.close();
		} catch (Exception exception) {
			exception.printStackTrace();
		}

		Vector<IExample> examples = new Vector<IExample>();
		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File(string)));
			String line = bufferedReader.readLine();
			while (line != null) {
				String[] strings = line.split(";");
				VideoExample videoExample = new VideoExample();
				videoExample.name = strings[0].split(":")[1].trim();
				videoExample.detectedMomentOfSnap = Integer.valueOf(strings[1]
						.split(":")[1].trim());
				videoExample.confidence = Float
						.valueOf(strings[2].split(":")[1].trim());
				videoExample.annotatedMomentOfSnap = videosMap
						.get(videoExample.name + ".avi").annotatedMOS;
				examples.add(videoExample);
				line = bufferedReader.readLine();
			}
			bufferedReader.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return examples;
	}

}
