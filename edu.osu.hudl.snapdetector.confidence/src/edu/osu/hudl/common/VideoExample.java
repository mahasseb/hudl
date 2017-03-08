package edu.osu.hudl.common;

import java.text.DecimalFormat;
import java.util.Vector;

public class VideoExample implements IExample {

	int label, predictedLabel;
	Vector<Float> x;
	float confidence;
	public Vector<LocalOptimum> localOptimums;

	@Override
	public int getLabel() {
		// TODO Auto-generated method stub
		return label;
	}

	@Override
	public Vector<Float> getX() {
		// TODO Auto-generated method stub
		return x;
	}

	public void setLabel(int label) {
		this.label = label;
	}

	@Override
	public int getPredictedLabel() {
		// TODO Auto-generated method stub
		return predictedLabel;
	}

	public void setPredictedLabel(int predictedLabel) {
		this.predictedLabel = predictedLabel;
	}

	public float getConfidence() {
		return confidence;
	}

	public void setConfidence(float confidence) {
		this.confidence = confidence;
	}

	/**
	 * 
	 */
	public String name;
	int baseLineMOS1;
	int baseLineMOS2;
	public int detectedMomentOfSnap;
	public int annotatedMomentOfSnap;
	float distance;
	float threshold;
	public float score;
	float score1;
	float score2;
	float score3;
	double motionBefore;
	double motionAfter;
	double motionBeforeT1;
	double motionAfterT1;
	double motionBeforeT2;
	double motionAfterT2;
	double motionInMinusT1Before;
	double motionInMinusT1After;
	double motionInPlusT1Before;
	double motionInPlusT1After;
	double motionInMinusT2Before;
	double motionInMinusT2After;
	double motionInPlusT2Before;
	double motionInPlusT2After;
	double noiseFeature;
	float weight;

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return name + ":" + "Snap=" + detectedMomentOfSnap + " D=" + distance
				+ " S1=" + score1 + " S2=" + score2 + " S3=" + score3
				+ " noise=" + noiseFeature;
	}

	@Override
	public String getLibSVMFormat() {
		StringBuffer stringBuffer = new StringBuffer();
		stringBuffer.append(label);
		stringBuffer.append(" 1:" + distance + " 2:" + score1 + " 3:" + score2
				+ " 4:" + score3 + " 5:" + motionBeforeT1 + " 6:"
				+ motionAfterT1 + " 7:" + motionBeforeT2 + " 8:"
				+ motionAfterT2 + " 9:" + motionInMinusT1Before + " 10:"
				+ motionInMinusT1After + " 11:" + motionInPlusT1Before + " 12:"
				+ motionInPlusT1After + " 13:" + motionInMinusT2Before + " 14:"
				+ motionInMinusT2After + " 15:" + motionInPlusT2Before + " 16:"
				+ motionInPlusT2After + "\n");// + " 17:" + noiseFeature +
												// "\n");
		return stringBuffer.toString();
	}

	public double getNoiseFeature() {
		return noiseFeature;
	}

	public void setNoiseFeature(double noiseFeature) {
		this.noiseFeature = noiseFeature;
	}

	public static float maxDistance = 3.0f;
	public static double maxMotionBeforeT1 = 96.0466f;
	public static double maxMotionAfterT1 = 190.802f;
	public static double maxMotionBeforeT2 = 43.1966f;
	public static double maxMotionAfterT2 = 114.591f;
	public static double maxMotionInMinusT1Before = 247.692f;
	public static double maxMotionInMinusT1After = 200.027f;
	public static double maxMotionInPlusT1Before = 234.283f;
	public static double maxMotionInPlusT1After = 192.707f;
	public static double maxMotionInMinusT2Before = 313.287f;
	public static double maxMotionInMinusT2After = 195.331;
	public static double maxMotionInPlusT2Before = 320.127;
	public static double maxMotionInPlusT2After = 222.987;
	public static float maxScore1 = 1;
	public static float maxScore2 = 1;
	public static float maxScore3 = 1;

	@Override
	public String getLossSensitiveFormat() {
		DecimalFormat df = new DecimalFormat("0");
		df.setDecimalSeparatorAlwaysShown(false);
		StringBuffer stringBuffer = new StringBuffer();
		stringBuffer.append(label);
		stringBuffer.append(" " + df.format(weight) + " |");
		stringBuffer.append(" 1:"
				+ df.format(distance / VideoExample.maxDistance)
				+ " 2:"
				+ df.format(score1 / VideoExample.maxScore1)
				+ " 3:"
				+ df.format(score2 / VideoExample.maxScore2)
				+ " 4:"
				+ df.format(score3 / VideoExample.maxScore3)
				+ " 5:"
				+ df.format(motionBeforeT1 / VideoExample.maxMotionBeforeT1)
				+ " 6:"
				+ df.format(motionAfterT1 / VideoExample.maxMotionAfterT1)
				+ " 7:"
				+ df.format(motionBeforeT2 / VideoExample.maxMotionBeforeT2)
				+ " 8:"
				+ df.format(motionAfterT2 / VideoExample.maxMotionAfterT2)
				+ " 9:"
				+ df.format(motionInMinusT1Before
						/ VideoExample.maxMotionInMinusT1Before)
				+ " 10:"
				+ df.format(motionInMinusT1After
						/ VideoExample.maxMotionInMinusT1After)
				+ " 11:"
				+ df.format(motionInPlusT1Before
						/ VideoExample.maxMotionInPlusT1Before)
				+ " 12:"
				+ df.format(motionInPlusT1After
						/ VideoExample.maxMotionInPlusT1After)
				+ " 13:"
				+ df.format(motionInMinusT2Before
						/ VideoExample.maxMotionInMinusT2Before)
				+ " 14:"
				+ df.format(motionInMinusT2After
						/ VideoExample.maxMotionInMinusT2After)
				+ " 15:"
				+ df.format(motionInPlusT2Before
						/ VideoExample.maxMotionInPlusT2Before)
				+ " 16:"
				+ df.format(motionInPlusT2After
						/ VideoExample.maxMotionInPlusT2After) + "\n");
		// stringBuffer.append(" 1:" + df.format(distance) + " 2:"
		// + df.format(score1) + " 3:" + df.format(score2) + " 4:"
		// + df.format(score3) + " 5:" + df.format(motionBeforeT1) + " 6:"
		// + df.format(motionAfterT1) + " 7:" + df.format(motionBeforeT2)
		// + " 8:" + df.format(motionAfterT2) + " 9:"
		// + df.format(motionInMinusT1Before) + " 10:"
		// + df.format(motionInMinusT1After) + " 11:"
		// + df.format(motionInPlusT1Before) + " 12:"
		// + df.format(motionInPlusT1After) + " 13:"
		// + df.format(motionInMinusT2Before) + " 14:"
		// + df.format(motionInMinusT2After) + " 15:"
		// + df.format(motionInPlusT2Before) + " 16:"
		// + df.format(motionInPlusT2After) + "\n");

		return stringBuffer.toString();
	}

	public float getWeight() {
		return weight;
	}

	public void setWeight(float weight) {
		this.weight = weight;
	}

	@Override
	public String getLossSensitiveSVMFormat(boolean flag) {
		DecimalFormat df = new DecimalFormat("0.##");
		df.setDecimalSeparatorAlwaysShown(false);
		StringBuffer stringBuffer = new StringBuffer();
		if (flag == true)
			stringBuffer.append(df.format(weight) + " ");
		// if (label == 1)
		// stringBuffer.append("+");
		stringBuffer.append(label);
		stringBuffer.append(" 1:"
				+ df.format(distance / VideoExample.maxDistance)
				+ " 2:"
				+ df.format(score1 / VideoExample.maxScore1)
				+ " 3:"
				+ df.format(score2 / VideoExample.maxScore2)
				+ " 4:"
				+ df.format(score3 / VideoExample.maxScore3)
				+ " 5:"
				+ df.format(motionBeforeT1 / VideoExample.maxMotionBeforeT1)
				+ " 6:"
				+ df.format(motionAfterT1 / VideoExample.maxMotionAfterT1)
				+ " 7:"
				+ df.format(motionBeforeT2 / VideoExample.maxMotionBeforeT2)
				+ " 8:"
				+ df.format(motionAfterT2 / VideoExample.maxMotionAfterT2)
				+ " 9:"
				+ df.format(motionInMinusT1Before
						/ VideoExample.maxMotionInMinusT1Before)
				+ " 10:"
				+ df.format(motionInMinusT1After
						/ VideoExample.maxMotionInMinusT1After)
				+ " 11:"
				+ df.format(motionInPlusT1Before
						/ VideoExample.maxMotionInPlusT1Before)
				+ " 12:"
				+ df.format(motionInPlusT1After
						/ VideoExample.maxMotionInPlusT1After)
				+ " 13:"
				+ df.format(motionInMinusT2Before
						/ VideoExample.maxMotionInMinusT2Before)
				+ " 14:"
				+ df.format(motionInMinusT2After
						/ VideoExample.maxMotionInMinusT2After)
				+ " 15:"
				+ df.format(motionInPlusT2Before
						/ VideoExample.maxMotionInPlusT2Before)
				+ " 16:"
				+ df.format(motionInPlusT2After
						/ VideoExample.maxMotionInPlusT2After) + "\n");

		return stringBuffer.toString();

	}

	@Override
	public String getLossSensitiveSVMFormat2(boolean flag) {
		DecimalFormat df = new DecimalFormat("0.##");
		df.setDecimalSeparatorAlwaysShown(false);
		StringBuffer stringBuffer = new StringBuffer();
		if (flag == true)
			stringBuffer.append(df.format(weight) + " ");
		// if (label == 1)
		// stringBuffer.append("+");

		stringBuffer.append(label);
		stringBuffer.append(" 1:"
				+ df.format(distance / VideoExample.maxDistance)
				+ " 2:"
				+ df.format(score1 / VideoExample.maxScore1)
				+ " 3:"
				+ df.format(score2 / VideoExample.maxScore2)
				+ " 4:"
				+ df.format(score3 / VideoExample.maxScore3)
				+ " 5:"
				+ df.format(motionBeforeT1 / VideoExample.maxMotionBeforeT1)
				+ " 6:"
				+ df.format(motionAfterT1 / VideoExample.maxMotionAfterT1)
				+ " 7:"
				+ df.format(motionBeforeT2 / VideoExample.maxMotionBeforeT2)
				+ " 8:"
				+ df.format(motionAfterT2 / VideoExample.maxMotionAfterT2)
				+ " 9:"
				+ df.format(motionInMinusT1Before
						/ VideoExample.maxMotionInMinusT1Before)
				+ " 10:"
				+ df.format(motionInMinusT1After
						/ VideoExample.maxMotionInMinusT1After)
				+ " 11:"
				+ df.format(motionInPlusT1Before
						/ VideoExample.maxMotionInPlusT1Before)
				+ " 12:"
				+ df.format(motionInPlusT1After
						/ VideoExample.maxMotionInPlusT1After)
				+ " 13:"
				+ df.format(motionInMinusT2Before
						/ VideoExample.maxMotionInMinusT2Before)
				+ " 14:"
				+ df.format(motionInMinusT2After
						/ VideoExample.maxMotionInMinusT2After)
				+ " 15:"
				+ df.format(motionInPlusT2Before
						/ VideoExample.maxMotionInPlusT2Before)
				+ " 16:"
				+ df.format(motionInPlusT2After
						/ VideoExample.maxMotionInPlusT2After));

		double maxDistance_1 = Float.MIN_VALUE;
		double maxDistance_2 = Float.MIN_VALUE;
		double maxMotionBeforeT1_1 = Float.MIN_VALUE;
		double maxMotionBeforeT1_2 = Float.MIN_VALUE;
		double maxMotionAfterT1_1 = Float.MIN_VALUE;
		double maxMotionAfterT1_2 = Float.MIN_VALUE;
		double maxMotionBeforeT2_1 = Float.MIN_VALUE;
		double maxMotionBeforeT2_2 = Float.MIN_VALUE;
		double maxMotionAfterT2_1 = Float.MIN_VALUE;
		double maxMotionAfterT2_2 = Float.MIN_VALUE;
		for (int i = 0; i < localOptimums.size(); i++) {
			LocalOptimum localOptimum = localOptimums.get(i);
			if (localOptimum.frame < detectedMomentOfSnap) {
				if (localOptimum.value > maxDistance_1)
					maxDistance_1 = localOptimum.value;
				if (localOptimum.motionBeforeT1 > maxMotionBeforeT1_1)
					maxMotionBeforeT1_1 = localOptimum.motionBeforeT1;
				if (localOptimum.motionAfterT1 > maxMotionAfterT1_1)
					maxMotionAfterT1_1 = localOptimum.motionAfterT1;
				if (localOptimum.motionBeforeT2 > maxMotionBeforeT2_1)
					maxMotionBeforeT2_1 = localOptimum.motionBeforeT2;
				if (localOptimum.motionAfterT2 > maxMotionAfterT2_1)
					maxMotionAfterT2_1 = localOptimum.motionAfterT2;
			} else {
				if (localOptimum.value > maxDistance_2)
					maxDistance_2 = localOptimum.value;
				if (localOptimum.motionBeforeT1 > maxMotionBeforeT1_2)
					maxMotionBeforeT1_2 = localOptimum.motionBeforeT1;
				if (localOptimum.motionAfterT1 > maxMotionAfterT1_2)
					maxMotionAfterT1_2 = localOptimum.motionAfterT1;
				if (localOptimum.motionBeforeT2 > maxMotionBeforeT2_2)
					maxMotionBeforeT2_2 = localOptimum.motionBeforeT2;
				if (localOptimum.motionAfterT2 > maxMotionAfterT2_2)
					maxMotionAfterT2_2 = localOptimum.motionAfterT2;
			}
		}
		stringBuffer.append(" 17:"
				+ df.format(maxDistance_1 / VideoExample.maxDistance));
		stringBuffer.append(" 18:"
				+ df.format(maxDistance_2 / VideoExample.maxDistance));
		stringBuffer.append(" 19:"
				+ df.format(maxMotionBeforeT1_1
						/ VideoExample.maxMotionBeforeT1));
		stringBuffer.append(" 20:"
				+ df.format(maxMotionBeforeT1_2
						/ VideoExample.maxMotionBeforeT1));
		stringBuffer
				.append(" 21:"
						+ df.format(maxMotionAfterT1_1
								/ VideoExample.maxMotionAfterT1));
		stringBuffer
				.append(" 22:"
						+ df.format(maxMotionAfterT1_2
								/ VideoExample.maxMotionAfterT1));
		stringBuffer.append(" 23:"
				+ df.format(maxMotionBeforeT2_1
						/ VideoExample.maxMotionBeforeT2));
		stringBuffer.append(" 24:"
				+ df.format(maxMotionBeforeT2_2
						/ VideoExample.maxMotionBeforeT2));
		stringBuffer
				.append(" 25:"
						+ df.format(maxMotionAfterT2_1
								/ VideoExample.maxMotionAfterT2));
		stringBuffer
				.append(" 26:"
						+ df.format(maxMotionAfterT2_2
								/ VideoExample.maxMotionAfterT2));
		stringBuffer.append('\n');
		return stringBuffer.toString();

	}

	@Override
	public String getId() {
		// TODO Auto-generated method stub
		return name;
	}

	@Override
	public int compareTo(IExample example) {
		return this.getId().compareTo(example.getId());
	}

	@Override
	public int getDirection() {
		int error = 15;
		if (detectedMomentOfSnap > annotatedMomentOfSnap + error) {
			return -1;
		} else if (detectedMomentOfSnap < annotatedMomentOfSnap - error) {
			return +1;
		} else {
			return 0;
		}
	}
}
