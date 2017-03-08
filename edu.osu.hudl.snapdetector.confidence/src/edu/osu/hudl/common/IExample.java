package edu.osu.hudl.common;

import java.util.Vector;

public interface IExample extends Comparable<IExample> {
	public String getId();

	public int getLabel();

	public float getConfidence();

	public int getPredictedLabel();

	public Vector<Float> getX();

	public String getLibSVMFormat();

	public String getLossSensitiveFormat();

	public String getLossSensitiveSVMFormat(boolean flag);
	public String getLossSensitiveSVMFormat2(boolean flag);

	public float getWeight();

	public void setWeight(float weight);

	public void setPredictedLabel(int predictedLabel);

	public int getDirection();
}
