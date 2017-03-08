package edu.osu.hudl.common;


import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

public class VideoExampleLoader {
	private Vector<IExample> positiveSamples;
	private Vector<IExample> negativeSamples;
	private Vector<IExample> trainingSamples;
	private Vector<IExample> validationSamples;
	private Vector<IExample> testSamples;
	public List<IExample> totalSamples;

	public VideoExampleLoader() {
		positiveSamples = new Vector<IExample>();
		negativeSamples = new Vector<IExample>();
		trainingSamples = new Vector<IExample>();
		validationSamples = new Vector<IExample>();
		testSamples = new Vector<IExample>();
		totalSamples = new Vector<IExample>();
	}

	public Vector<IExample> getPositiveSamples() {
		return positiveSamples;
	}

	public void setPositiveSamples(Vector<IExample> positiveSamples) {
		this.positiveSamples = positiveSamples;
	}

	public Vector<IExample> getNegativeSamples() {
		return negativeSamples;
	}

	public void setNegativeSamples(Vector<IExample> negativeSamples) {
		this.negativeSamples = negativeSamples;
	}

	public Vector<IExample> getTrainingSamples() {
		return trainingSamples;
	}

	public void setTrainingSamples(Vector<IExample> trainingSamples) {
		this.trainingSamples = trainingSamples;
	}

	public Vector<IExample> getTestSamples() {
		return testSamples;
	}

	public void setTestSamples(Vector<IExample> testSamples) {
		this.testSamples = testSamples;
	}

	public Vector<IExample> getValidationSamples() {
		return validationSamples;
	}

	public void setValidationSamples(Vector<IExample> validationSamples) {
		this.validationSamples = validationSamples;
	}

	public void loadGroundTruth(String groundTruthFileName,
			List<IExample> totalSamples) {
		HashMap<String, Integer> starts = new HashMap<String, Integer>();
		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File(groundTruthFileName)));
			String line = bufferedReader.readLine();
			while (line != null) {
				String[] strings = line.split(";");

				String videoName = strings[1].trim();
				int groundTruthMOS = Integer.valueOf(strings[2]);
				starts.put(videoName, groundTruthMOS);
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
		for (IExample iExample : totalSamples) {
			if (starts.get(iExample.getId()) != null) {
				VideoExample videoExample = (VideoExample) iExample;
				videoExample.annotatedMomentOfSnap = starts.get(iExample
						.getId());

			} else {
				VideoExample videoExample = (VideoExample) iExample;
				videoExample.annotatedMomentOfSnap = 0;
			}
		}
		return;
	}

	public void loadSamples(ClassificationType classificationType,
			String detectedMOSFileName, String groundTruthFileName) {

		Vector<IExample> samples = new Vector<IExample>();
		File file = new File(detectedMOSFileName);
		System.out.println("Loading " + detectedMOSFileName + " = "
				+ file.exists());
		Vector<LocalOptimum> localOptimums = null;
		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					file));
			String line = null;
			short state = -2;
			VideoExample video = null;
			String[] strings;
			do {
				LocalOptimum localOptimum;
				line = bufferedReader.readLine();
				if (line == null)
					break;
				switch (state) {
				case -2:
					if (!line.startsWith("Local Maximums:")) {
						state = -2;
						break;
					}
					localOptimums = new Vector<LocalOptimum>();
					state = -1;
					break;
				case -1:
					if (line.equalsIgnoreCase("-----------------")) {
						state = 0;
						break;
					}
					strings = line.split(":");
					localOptimum = new LocalOptimum();
					localOptimum.frame = Integer.valueOf(strings[0]);
					localOptimum.value = Float.valueOf(strings[2]);
					line = bufferedReader.readLine();
					line = bufferedReader.readLine();
					if (!line.startsWith("BF")) {
						state = -2;
						break;
					}
					strings = line.split(";");
					String[] subString;
					subString = strings[0].split(":");
					localOptimum.motionBeforeT1 = Double.valueOf(subString[1]
							.trim());
					subString = strings[1].split(":");
					localOptimum.motionAfterT1 = Double.valueOf(subString[1]
							.trim());
					subString = strings[2].split(":");
					localOptimum.motionBeforeT2 = Double.valueOf(subString[1]
							.trim());
					subString = strings[3].split(":");
					localOptimum.motionAfterT2 = Double.valueOf(subString[1]
							.trim());
					subString = strings[4].split(":");
					localOptimum.motionInMinusT1Before = Double
							.valueOf(subString[1].trim());
					subString = strings[5].split(":");
					localOptimum.motionInMinusT1After = Double
							.valueOf(subString[1].trim());
					subString = strings[6].split(":");
					localOptimum.motionInPlusT1Before = Double
							.valueOf(subString[1].trim());
					subString = strings[7].split(":");
					localOptimum.motionInPlusT1After = Double
							.valueOf(subString[1].trim());
					subString = strings[8].split(":");
					localOptimum.motionInMinusT2Before = Double
							.valueOf(subString[1].trim());
					subString = strings[9].split(":");
					localOptimum.motionInMinusT2After = Double
							.valueOf(subString[1].trim());
					subString = strings[10].split(":");
					localOptimum.motionInPlusT2Before = Double
							.valueOf(subString[1].trim());
					subString = strings[11].split(":");
					localOptimum.motionInPlusT2After = Double
							.valueOf(subString[1].trim());
					localOptimums.add(localOptimum);
					break;
				case 0:
					if (!line.startsWith("Video Name")) {
						state = 0;
						break;
					}
					video = new VideoExample();
					video.name = line.split(":")[1].trim();
					video.localOptimums = localOptimums;
					state = 1;
					break;
				case 1:
					if (!line.startsWith("Moment of Snap")) {
						state = 0;
						break;
					}
					video.detectedMomentOfSnap = Integer.valueOf(line
							.split(":")[1].trim());
					state = 2;
					break;

				case 2:
					if (!line.startsWith("Max Distance")) {
						state = 0;
						break;
					}
					video.distance = Float.valueOf(line.split(":")[1].trim());
					state = 3;
					break;
				case 3:
					if (!line.startsWith("Threshold")) {
						state = 0;
						break;

					}
					video.threshold = Float.valueOf(line.split(":")[1].trim());
					state = 4;
					break;

				case 4:
					if (!line.startsWith("Confidence")) {
						state = 0;
						break;
					}
					strings = line.split(";");
					video.confidence = Float.valueOf(strings[0].split(":")[1]
							.trim());
					video.score1 = Float.valueOf(strings[1].split(":")[1]
							.trim());
					video.score2 = Float.valueOf(strings[2].split(":")[1]
							.trim());

					video.score3 = Float.valueOf(strings[3].split(":")[1]
							.trim());
					state = 5;
					break;
				case 5:
					if (!line.startsWith("BF")) {
						state = 0;
						break;
					}
					strings = line.split(";");
					subString = strings[0].split(":");
					video.motionBeforeT1 = Double.valueOf(subString[1].trim());
					subString = strings[1].split(":");
					video.motionAfterT1 = Double.valueOf(subString[1].trim());
					subString = strings[2].split(":");
					video.motionBeforeT2 = Double.valueOf(subString[1].trim());
					subString = strings[3].split(":");
					video.motionAfterT2 = Double.valueOf(subString[1].trim());
					subString = strings[4].split(":");
					video.motionInMinusT1Before = Double.valueOf(subString[1]
							.trim());
					subString = strings[5].split(":");
					video.motionInMinusT1After = Double.valueOf(subString[1]
							.trim());
					subString = strings[6].split(":");
					video.motionInPlusT1Before = Double.valueOf(subString[1]
							.trim());
					subString = strings[7].split(":");
					video.motionInPlusT1After = Double.valueOf(subString[1]
							.trim());
					subString = strings[8].split(":");
					video.motionInMinusT2Before = Double.valueOf(subString[1]
							.trim());
					subString = strings[9].split(":");
					video.motionInMinusT2After = Double.valueOf(subString[1]
							.trim());
					subString = strings[10].split(":");
					video.motionInPlusT2Before = Double.valueOf(subString[1]
							.trim());
					subString = strings[11].split(":");
					video.motionInPlusT2After = Double.valueOf(subString[1]
							.trim());
					samples.add(video);
					state = 0;
				default:
					break;
				}
			} while (true);
			bufferedReader.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		totalSamples = samples;
		Collections.sort(totalSamples);
		file = new File(groundTruthFileName);
		loadGroundTruth(groundTruthFileName, totalSamples);

		for (IExample sample : samples) {
			VideoExample videoSample = (VideoExample) sample;
			if (videoSample.detectedMomentOfSnap == 0)
				continue;
			int error = Math.abs(videoSample.detectedMomentOfSnap
					- videoSample.annotatedMomentOfSnap);
			if (error <= 15) {
				videoSample.setLabel(1);
				positiveSamples.add(videoSample);
			} else if (error <= 30) {
				if (classificationType == ClassificationType.class1) {
					videoSample.setLabel(-1);
					negativeSamples.add(videoSample);
				} else {
					videoSample.setLabel(1);
					positiveSamples.add(videoSample);
				}
			} else {
				videoSample.setLabel(-1);
				negativeSamples.add(videoSample);
			}

			if (VideoExample.maxDistance < videoSample.distance)
				VideoExample.maxDistance = videoSample.distance;
			if (VideoExample.maxScore1 < videoSample.score1)
				VideoExample.maxScore1 = videoSample.score1;
			if (VideoExample.maxScore2 < videoSample.score2)
				VideoExample.maxScore2 = videoSample.score2;
			if (VideoExample.maxScore3 < videoSample.score3)
				VideoExample.maxScore3 = videoSample.score3;
			if (VideoExample.maxMotionBeforeT1 < videoSample.motionBeforeT1)
				VideoExample.maxMotionBeforeT1 = videoSample.motionBeforeT1;
			if (VideoExample.maxMotionAfterT1 < videoSample.motionAfterT1)
				VideoExample.maxMotionAfterT1 = videoSample.motionAfterT1;
			if (VideoExample.maxMotionBeforeT2 < videoSample.motionBeforeT2)
				VideoExample.maxMotionBeforeT2 = videoSample.motionBeforeT2;
			if (VideoExample.maxMotionAfterT2 < videoSample.motionAfterT2)
				VideoExample.maxMotionAfterT2 = videoSample.motionAfterT2;
			if (VideoExample.maxMotionInMinusT1Before < videoSample.motionInMinusT1Before)
				VideoExample.maxMotionInMinusT1Before = videoSample.motionInMinusT1Before;
			if (VideoExample.maxMotionInMinusT1After < videoSample.motionInMinusT1After)
				VideoExample.maxMotionInMinusT1After = videoSample.motionInMinusT1After;
			if (VideoExample.maxMotionInPlusT1Before < videoSample.motionInPlusT1Before)
				VideoExample.maxMotionInPlusT1Before = videoSample.motionInPlusT1Before;
			if (VideoExample.maxMotionInPlusT1After < videoSample.motionInPlusT1After)
				VideoExample.maxMotionInPlusT1After = videoSample.motionInPlusT1After;
			if (VideoExample.maxMotionInMinusT2Before < videoSample.motionInMinusT2Before)
				VideoExample.maxMotionInMinusT2Before = videoSample.motionInMinusT2Before;
			if (VideoExample.maxMotionInMinusT2After < videoSample.motionInMinusT2After)
				VideoExample.maxMotionInMinusT2After = videoSample.motionInMinusT2After;
			if (VideoExample.maxMotionInPlusT2Before < videoSample.motionInPlusT2Before)
				VideoExample.maxMotionInPlusT2Before = videoSample.motionInPlusT2Before;
			if (VideoExample.maxMotionInPlusT2After < videoSample.motionInPlusT2After)
				VideoExample.maxMotionInPlusT2After = videoSample.motionInPlusT2After;
		}
	}

	static String TRAINING_SAVED = "training_saved.txt";
	static String VALIDATION_SAVED = "validation_saved.txt";
	static String TESTING_SAVED = "testing_saved.txt";

	public void generateDataset(float train_test_percent,
			float train_validation_percent) {
		Collections.shuffle(positiveSamples);
		Collections.shuffle(negativeSamples);
		for (int i = 0; i < positiveSamples.size(); i++) {
			if (i < positiveSamples.size() * train_test_percent) {
				if (i < positiveSamples.size() * train_test_percent
						* train_validation_percent)
					trainingSamples.add(positiveSamples.get(i));
				else
					validationSamples.add(positiveSamples.get(i));
			} else {
				testSamples.add(positiveSamples.get(i));
			}
		}
		for (int i = 0; i < negativeSamples.size(); i++) {
			if (i < negativeSamples.size() * train_test_percent) {
				if (i < negativeSamples.size() * train_test_percent
						* train_validation_percent)
					trainingSamples.add(negativeSamples.get(i));
				else
					validationSamples.add(negativeSamples.get(i));
			} else {
				testSamples.add(negativeSamples.get(i));
			}
		}
		Collections.shuffle(trainingSamples);
		Collections.shuffle(testSamples);
	}

	public void saveLoadedSamples() {
		File trainingFile = new File(TRAINING_SAVED);
		if (trainingFile.exists())
			trainingFile.delete();
		File testingFile = new File(TESTING_SAVED);
		if (testingFile.exists())
			testingFile.delete();
		File validationFile = new File(TESTING_SAVED);
		if (validationFile.exists())
			validationFile.delete();
		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File(TRAINING_SAVED)));
			for (IExample iSample : trainingSamples) {
				bufferedWriter.write(iSample.getLibSVMFormat());
				bufferedWriter.flush();
			}
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File(VALIDATION_SAVED)));
			for (IExample iSample : validationSamples) {
				bufferedWriter.write(iSample.getLibSVMFormat());
				bufferedWriter.flush();
			}
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File(TESTING_SAVED)));
			for (IExample iSample : testSamples) {
				bufferedWriter.write(iSample.getLibSVMFormat());
				bufferedWriter.flush();
			}
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public void loadSavedSamples() {
		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File(TRAINING_SAVED)));
			String line = bufferedReader.readLine();
			trainingSamples = new Vector<IExample>();
			while (line != null) {
				String[] strings = line.split(" ");
				VideoExample videoSample = new VideoExample();
				videoSample.setLabel(Integer.valueOf(strings[0]));
				videoSample.distance = Float.valueOf(strings[1].split(":")[1]);
				videoSample.score1 = Float.valueOf(strings[2].split(":")[1]);
				videoSample.score2 = Float.valueOf(strings[3].split(":")[1]);
				videoSample.score3 = Float.valueOf(strings[4].split(":")[1]);
				videoSample.motionBeforeT1 = Float.valueOf(strings[5]
						.split(":")[1]);
				videoSample.motionAfterT1 = Float
						.valueOf(strings[6].split(":")[1]);
				videoSample.motionBeforeT2 = Float.valueOf(strings[7]
						.split(":")[1]);
				videoSample.motionAfterT2 = Float
						.valueOf(strings[8].split(":")[1]);
				videoSample.motionInMinusT1Before = Float.valueOf(strings[9]
						.split(":")[1]);
				videoSample.motionInMinusT1After = Float.valueOf(strings[10]
						.split(":")[1]);
				videoSample.motionInPlusT1Before = Float.valueOf(strings[11]
						.split(":")[1]);
				videoSample.motionInPlusT1After = Float.valueOf(strings[12]
						.split(":")[1]);
				videoSample.motionInMinusT2Before = Float.valueOf(strings[13]
						.split(":")[1]);
				videoSample.motionInMinusT2After = Float.valueOf(strings[14]
						.split(":")[1]);
				videoSample.motionInPlusT2Before = Float.valueOf(strings[15]
						.split(":")[1]);
				videoSample.motionInPlusT2After = Float.valueOf(strings[16]
						.split(":")[1]);
				if (VideoExample.maxDistance < videoSample.distance)
					VideoExample.maxDistance = videoSample.distance;
				if (VideoExample.maxScore1 < videoSample.score1)
					VideoExample.maxScore1 = videoSample.score1;
				if (VideoExample.maxScore2 < videoSample.score2)
					VideoExample.maxScore2 = videoSample.score2;
				if (VideoExample.maxScore3 < videoSample.score3)
					VideoExample.maxScore3 = videoSample.score3;
				if (VideoExample.maxMotionBeforeT1 < videoSample.motionBeforeT1)
					VideoExample.maxMotionBeforeT1 = videoSample.motionBeforeT1;
				if (VideoExample.maxMotionAfterT1 < videoSample.motionAfterT1)
					VideoExample.maxMotionAfterT1 = videoSample.motionAfterT1;
				if (VideoExample.maxMotionBeforeT2 < videoSample.motionBeforeT2)
					VideoExample.maxMotionBeforeT2 = videoSample.motionBeforeT2;
				if (VideoExample.maxMotionAfterT2 < videoSample.motionAfterT2)
					VideoExample.maxMotionAfterT2 = videoSample.motionAfterT2;
				if (VideoExample.maxMotionInMinusT1Before < videoSample.motionInMinusT1Before)
					VideoExample.maxMotionInMinusT1Before = videoSample.motionInMinusT1Before;
				if (VideoExample.maxMotionInMinusT1After < videoSample.motionInMinusT1After)
					VideoExample.maxMotionInMinusT1After = videoSample.motionInMinusT1After;
				if (VideoExample.maxMotionInPlusT1Before < videoSample.motionInPlusT1Before)
					VideoExample.maxMotionInPlusT1Before = videoSample.motionInPlusT1Before;
				if (VideoExample.maxMotionInPlusT1After < videoSample.motionInPlusT1After)
					VideoExample.maxMotionInPlusT1After = videoSample.motionInPlusT1After;
				if (VideoExample.maxMotionInMinusT2Before < videoSample.motionInMinusT2Before)
					VideoExample.maxMotionInMinusT2Before = videoSample.motionInMinusT2Before;
				if (VideoExample.maxMotionInMinusT2After < videoSample.motionInMinusT2After)
					VideoExample.maxMotionInMinusT2After = videoSample.motionInMinusT2After;
				if (VideoExample.maxMotionInPlusT2Before < videoSample.motionInPlusT2Before)
					VideoExample.maxMotionInPlusT2Before = videoSample.motionInPlusT2Before;
				if (VideoExample.maxMotionInPlusT2After < videoSample.motionInPlusT2After)
					VideoExample.maxMotionInPlusT2After = videoSample.motionInPlusT2After;

				trainingSamples.add(videoSample);
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
		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File(TESTING_SAVED)));
			String line = bufferedReader.readLine();
			testSamples = new Vector<IExample>();
			while (line != null) {
				String[] strings = line.split(" ");
				VideoExample videoSample = new VideoExample();
				videoSample.setLabel(Integer.valueOf(strings[0]));
				videoSample.distance = Float.valueOf(strings[1].split(":")[1]);
				videoSample.score1 = Float.valueOf(strings[2].split(":")[1]);
				videoSample.score2 = Float.valueOf(strings[3].split(":")[1]);
				videoSample.score3 = Float.valueOf(strings[4].split(":")[1]);
				videoSample.motionBeforeT1 = Float.valueOf(strings[5]
						.split(":")[1]);
				videoSample.motionAfterT1 = Float
						.valueOf(strings[6].split(":")[1]);
				videoSample.motionBeforeT2 = Float.valueOf(strings[7]
						.split(":")[1]);
				videoSample.motionAfterT2 = Float
						.valueOf(strings[8].split(":")[1]);
				videoSample.motionInMinusT1Before = Float.valueOf(strings[9]
						.split(":")[1]);
				videoSample.motionInMinusT1After = Float.valueOf(strings[10]
						.split(":")[1]);
				videoSample.motionInPlusT1Before = Float.valueOf(strings[11]
						.split(":")[1]);
				videoSample.motionInPlusT1After = Float.valueOf(strings[12]
						.split(":")[1]);
				videoSample.motionInMinusT2Before = Float.valueOf(strings[13]
						.split(":")[1]);
				videoSample.motionInMinusT2After = Float.valueOf(strings[14]
						.split(":")[1]);
				videoSample.motionInPlusT2Before = Float.valueOf(strings[15]
						.split(":")[1]);
				videoSample.motionInPlusT2After = Float.valueOf(strings[16]
						.split(":")[1]);
				if (VideoExample.maxDistance < videoSample.distance)
					VideoExample.maxDistance = videoSample.distance;
				if (VideoExample.maxScore1 < videoSample.score1)
					VideoExample.maxScore1 = videoSample.score1;
				if (VideoExample.maxScore2 < videoSample.score2)
					VideoExample.maxScore2 = videoSample.score2;
				if (VideoExample.maxScore3 < videoSample.score3)
					VideoExample.maxScore3 = videoSample.score3;
				if (VideoExample.maxMotionBeforeT1 < videoSample.motionBeforeT1)
					VideoExample.maxMotionBeforeT1 = videoSample.motionBeforeT1;
				if (VideoExample.maxMotionAfterT1 < videoSample.motionAfterT1)
					VideoExample.maxMotionAfterT1 = videoSample.motionAfterT1;
				if (VideoExample.maxMotionBeforeT2 < videoSample.motionBeforeT2)
					VideoExample.maxMotionBeforeT2 = videoSample.motionBeforeT2;
				if (VideoExample.maxMotionAfterT2 < videoSample.motionAfterT2)
					VideoExample.maxMotionAfterT2 = videoSample.motionAfterT2;
				if (VideoExample.maxMotionInMinusT1Before < videoSample.motionInMinusT1Before)
					VideoExample.maxMotionInMinusT1Before = videoSample.motionInMinusT1Before;
				if (VideoExample.maxMotionInMinusT1After < videoSample.motionInMinusT1After)
					VideoExample.maxMotionInMinusT1After = videoSample.motionInMinusT1After;
				if (VideoExample.maxMotionInPlusT1Before < videoSample.motionInPlusT1Before)
					VideoExample.maxMotionInPlusT1Before = videoSample.motionInPlusT1Before;
				if (VideoExample.maxMotionInPlusT1After < videoSample.motionInPlusT1After)
					VideoExample.maxMotionInPlusT1After = videoSample.motionInPlusT1After;
				if (VideoExample.maxMotionInMinusT2Before < videoSample.motionInMinusT2Before)
					VideoExample.maxMotionInMinusT2Before = videoSample.motionInMinusT2Before;
				if (VideoExample.maxMotionInMinusT2After < videoSample.motionInMinusT2After)
					VideoExample.maxMotionInMinusT2After = videoSample.motionInMinusT2After;
				if (VideoExample.maxMotionInPlusT2Before < videoSample.motionInPlusT2Before)
					VideoExample.maxMotionInPlusT2Before = videoSample.motionInPlusT2Before;
				if (VideoExample.maxMotionInPlusT2After < videoSample.motionInPlusT2After)
					VideoExample.maxMotionInPlusT2After = videoSample.motionInPlusT2After;

				testSamples.add(videoSample);
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
	}
}
