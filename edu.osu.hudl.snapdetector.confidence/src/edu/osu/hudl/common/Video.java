package edu.osu.hudl.common;

import java.util.Vector;

public class Video {
	public int id;
	public String name;
	public int annotatedMOS;
	int opticalflowRunningTime;
	int cellProcessingRunningTime;
	int vtiProcessingRunningTime;
	int videoLength;
	int totalRunningTime;
	public Vector<Integer> detectedMOSs;
	public int detectedMOS = -10;
	public Vector<LocalOptimum> localOptimums;
	public Vector<Integer> frames;
	public Vector<Float> thresholds;
	public Vector<Vector<Float>> vti;
	float confidence;

	public Video() {
		// TODO Auto-generated constructor stub
		frames = new Vector<Integer>();
		thresholds = new Vector<Float>();
		vti = new Vector<Vector<Float>>();
	}

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "Video:" + name + " AMOS-> " + annotatedMOS + " DMOS-> "
				+ detectedMOS;
	}
}
