package edu.osu.hudl.mos.confidence;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Vector;

import edu.osu.hudl.common.ClassificationResult;
import edu.osu.hudl.common.IClassifier;
import edu.osu.hudl.common.IExample;
import edu.osu.hudl.common.VideoExample;

public class SVMBasedLossSensitiveClassifier implements IClassifier {

	float lambda;
	float t;
	boolean normalizeWeight;
	boolean all;
	boolean weighted = true;

	public SVMBasedLossSensitiveClassifier(boolean normalizeWeight, boolean all) {
		this.normalizeWeight = normalizeWeight;
		this.all = all;
	}

	@Override
	public void learn(Vector<IExample> examples) {
		String[] commands = { "./svm-train", "train", "train.model" };
		// String[] commands = { "./svm-train", "-W", "1", "train",
		// "train.model" };
		if (!weighted) {
			try {
				// commands = commands1;
				BufferedWriter bufferedWriter = new BufferedWriter(
						new FileWriter(new File("train")));
				for (int i = 0; i < examples.size(); i++) {
					float weight = 0.76f;
					if (examples.get(i).getLabel() == 1) {
						weight = 1.0f;
					} else if (examples.get(i).getLabel() == 2) {
						weight = 0.75f;
					} else {
						weight = 0.55f;
					}
					examples.get(i).setWeight(weight);
					if (all) {
						examples.get(i).setWeight(weight);
					} else {
						if (examples.get(i).getLabel() != examples.get(i)
								.getPredictedLabel()) {
							examples.get(i).setWeight(weight);
						} else {
							if (normalizeWeight)
								examples.get(i).setWeight(0);
							else
								examples.get(i).setWeight(1);
						}
					}
					bufferedWriter.write(examples.get(i)
							.getLossSensitiveSVMFormat2(true));
				}
				bufferedWriter.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			Runtime runtime = Runtime.getRuntime();
			try {
				Process process = runtime.exec(commands);
				BufferedReader bufferedReader = new BufferedReader(
						new InputStreamReader(process.getInputStream()));
				String line;
				line = bufferedReader.readLine();
				while (line != null) {
					line = bufferedReader.readLine();
				}
				bufferedReader.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			return;
		}
		float epsilon = (1 - t) / t;
		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File("train")));
			for (int i = 0; i < examples.size(); i++) {
				float weight = 1;
				if (examples.get(i).getLabel() == 1) {
					if (normalizeWeight)
						weight = (1 + epsilon * lambda)
								/ (1 + lambda + epsilon * lambda);
					else
						weight = (1 + epsilon * lambda);
					// examples.get(i).setWeight(
					// (1 + epsilon * lambda)
					// / (1 + lambda + epsilon * lambda));
				} else {
					if (normalizeWeight) {
						weight = (lambda) / (1 + lambda + epsilon * lambda);
					} else {
						weight = lambda;
					}
					// examples.get(i).setWeight(
					// (lambda) / (1 + lambda + epsilon * lambda));
				}
				if (all) {
					examples.get(i).setWeight(weight);
				} else {
					if (examples.get(i).getLabel() != examples.get(i)
							.getPredictedLabel()) {
						examples.get(i).setWeight(weight);
					} else {
						if (normalizeWeight)
							examples.get(i).setWeight(0);
						else
							examples.get(i).setWeight(1);
					}
				}
				bufferedWriter.write(examples.get(i).getLossSensitiveSVMFormat(
						false));
			}
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		Runtime runtime = Runtime.getRuntime();
		// String[] commands = { "./svm-train", "-W", "1", "train",
		// "train.model" };
		try {
			Process process = runtime.exec(commands);
			BufferedReader bufferedReader = new BufferedReader(
					new InputStreamReader(process.getInputStream()));
			String line;
			line = bufferedReader.readLine();
			while (line != null) {
				line = bufferedReader.readLine();
			}
			bufferedReader.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public ClassificationResult predict(IExample example) {
		ClassificationResult result = new ClassificationResult();
		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File("test")));
			bufferedWriter.write(example.getLossSensitiveSVMFormat(false));
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Runtime runtime = Runtime.getRuntime();
		String[] commands = { "./svm-predict", "test", "train.model",
				"prediction.txt" };
		try {
			Process process = runtime.exec(commands);
			BufferedReader bufferedReader = new BufferedReader(
					new InputStreamReader(process.getErrorStream()));
			String line;
			line = bufferedReader.readLine();
			while (line != null) {
				line = bufferedReader.readLine();
			}
			bufferedReader.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		try {
			BufferedReader bufferedReader;
			bufferedReader = new BufferedReader(new FileReader(new File(
					"prediction.txt")));
			String line = bufferedReader.readLine();
			while (line != null) {
				result.label = Math.round(Float.valueOf(line.split(" ")[0]));
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
		return result;
	}

	@Override
	public ClassificationResult predict(Vector<IExample> examples, boolean flag) {
		// TODO Auto-generated method stub
		ClassificationResult result = new ClassificationResult();
		try {
			BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(
					new File("test")));
			for (int i = 0; i < examples.size(); i++) {
				bufferedWriter.write(examples.get(i).getLossSensitiveSVMFormat(
						false));
			}
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Runtime runtime = Runtime.getRuntime();
		String[] commands = { "./svm-predict", "test", "train.model",
				"prediction.txt" };
		try {
			Process process = runtime.exec(commands);
			BufferedReader bufferedReader = new BufferedReader(
					new InputStreamReader(process.getErrorStream()));
			String line;
			line = bufferedReader.readLine();
			while (line != null) {
				line = bufferedReader.readLine();
			}
			bufferedReader.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(
					new File("prediction.txt")));
			String line = bufferedReader.readLine();
			int index = 0;
			while (line != null) {
				VideoExample videoExample = (VideoExample) examples.get(index);
				examples.get(index).setPredictedLabel(
						Math.round(Float.valueOf(line.split(" ")[0])));

				if (flag == true) {
					if (Math.abs(videoExample.annotatedMomentOfSnap
							- videoExample.detectedMomentOfSnap) <= 15) {
						if (videoExample.getConfidence() > 0.5) {
							System.out.println("1,1,"
									+ videoExample.getPredictedLabel());
						} else {
							System.out.println("1,-1,"
									+ videoExample.getPredictedLabel());
						}
					} else {
						if (videoExample.getConfidence() > 0.5) {
							System.out.println("-1,1,"
									+ videoExample.getPredictedLabel());
						} else {
							System.out.println("-1,-1,"
									+ videoExample.getPredictedLabel());
						}

					}
				}
				line = bufferedReader.readLine();
				index++;
			}
			bufferedReader.close();
			float tp = 0;
			float tn = 0;
			float fp = 0;
			float fn = 0;
			for (IExample example : examples) {
				if (example.getLabel() == 1) {
					if (example.getPredictedLabel() == 1) {
						tp++;
					} else {
						fn++;
					}
				} else {
					if (example.getPredictedLabel() == 1) {
						fp++;
					} else {
						tn++;
					}
				}
			}
			result.precision = Float.NaN;
			result.recall = Float.NaN;
			if (tp + fp != 0) {
				result.precision = tp / (tp + fp);
			}
			if (tp + fn != 0) {

				result.recall = tp / (tp + fn);
			}
			result.fn = fn;
			result.fp = fp;
			result.tn = tn;
			result.tp = tp;
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		result.outputs = examples;
		return result;
	}

	@Override
	public ClassificationResult predict(Vector<IExample> examples) {
		// TODO Auto-generated method stub
		return predict(examples, false);
	}

	@Override
	public ClassificationResult predict2(Vector<IExample> examples) {
		// TODO Auto-generated method stub
		return predict(examples, true);
	}

}
