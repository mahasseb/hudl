package edu.osu.hudl.mos.confidence;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

import edu.osu.hudl.common.ClassificationResult;
import edu.osu.hudl.common.ClassificationType;
import edu.osu.hudl.common.VideoExampleLoader;

public class ConfidenceEstimator {
	public static void learn(String[] args) {
		System.out.println("Learning the Confidence Estimator");
		VideoExampleLoader videoExampleLoader = new VideoExampleLoader();
		videoExampleLoader.loadSamples(ClassificationType.class1,
				"profile.log", "games_mapping.txt");
		videoExampleLoader.generateDataset(0.75f, 0.25f);
		SVMBasedLossSensitiveClassifier classifier = new SVMBasedLossSensitiveClassifier(
				false, true);
//		classifier.learn(videoExampleLoader.getTrainingSamples());
		ClassificationResult result = classifier.predict(videoExampleLoader
				.getTestSamples());
		System.out.println(result);
	}

	public static void predict(String[] args) {
		VideoExampleLoader videoExampleLoader = new VideoExampleLoader();
		videoExampleLoader.loadSamples(ClassificationType.class1,
				".profile.log", "");
		Runtime runtime = Runtime.getRuntime();
		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File("test")));
			for (int i = 0; i < videoExampleLoader.totalSamples.size(); i++) {
				bufferedWriter.write(videoExampleLoader.totalSamples.get(i)
						.getLossSensitiveSVMFormat(false));
			}
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String[] commands = { "./svm-predict", "test", "train.model",
				"prediction.txt" };
		Process process = null;
		try {
			process = runtime.exec(commands);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			BufferedReader bufferedReader = new BufferedReader(
					new InputStreamReader(process.getErrorStream()));
			String line;
			line = bufferedReader.readLine();
			while (line != null) {
				line = bufferedReader.readLine();
				System.err.println(line);
			}
			bufferedReader = new BufferedReader(new InputStreamReader(
					process.getInputStream()));
			line = bufferedReader.readLine();
			while (line != null) {
				line = bufferedReader.readLine();
				// System.out.println(line);
			}
			bufferedReader.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			// System.out.println(new File("prediction.txt").exists());
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File("prediction.txt")));
			String line1 = bufferedReader.readLine();
			bufferedReader.close();
			bufferedReader = new BufferedReader(new FileReader(new File(
					".mos_output.txt")));
			String line2 = bufferedReader.readLine();
			line2 += "; New Confidence: " + line1 + "\n";
			bufferedReader.close();
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File(".confidence_output.txt")));
			bufferedWriter.write(line2);
			bufferedWriter.flush();
			bufferedWriter.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public static void main(String[] args) {
		learn(args);
		// predict(args);
	}
}
