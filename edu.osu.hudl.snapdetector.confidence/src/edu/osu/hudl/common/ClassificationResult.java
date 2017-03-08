package edu.osu.hudl.common;

import java.util.Vector;

public class ClassificationResult {
	public int label;
	// ///////////////////
	public float accuracy;
	public float precision;
	public float recall;
	public float fp, fn, tp, tn;

	public Vector<IExample> outputs;

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "Recall:" + recall + ",Precision:" + precision;
	}
}
