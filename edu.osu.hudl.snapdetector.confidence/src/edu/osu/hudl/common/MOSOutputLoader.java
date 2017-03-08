package edu.osu.hudl.common;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.List;
import java.util.Vector;

public class MOSOutputLoader {

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
						.valueOf(strings[6].split(":")[1].trim());
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
