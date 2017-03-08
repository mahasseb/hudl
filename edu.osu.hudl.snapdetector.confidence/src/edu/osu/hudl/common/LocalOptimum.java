package edu.osu.hudl.common;

public class LocalOptimum implements Comparable<LocalOptimum> {

	public double motionBefore;
	public double motionAfter;
	public double motionBeforeT1;
	public double motionAfterT1;
	public double motionBeforeT2;
	public double motionAfterT2;
	public double motionInMinusT1Before;
	public double motionInMinusT1After;
	public double motionInPlusT1Before;
	public double motionInPlusT1After;
	public double motionInMinusT2Before;
	public double motionInMinusT2After;
	public double motionInPlusT2Before;
	public double motionInPlusT2After;

	public LocalOptimum() {
		// TODO Auto-generated constructor stub
	}

	public LocalOptimum(int frame, float threshold, int frameIndex,
			int thresholdIndex, float value) {
		this.frame = frame;
		this.threshold = threshold;
		this.value = value;
		this.frameIndex = frameIndex;
		this.thresholdIndex = thresholdIndex;
	}

	public int frame;
	public float threshold;
	public float value;
	public int thresholdIndex;
	public int frameIndex;

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "F:" + frame + ", T:" + threshold + ", Dst:" + value
				+ ", BF T1L" + motionBeforeT1;
	}

	@Override
	public int compareTo(LocalOptimum localOptimum) {
		if (frame < localOptimum.frame)
			return -1;
		else if (frame == localOptimum.frame) {
			return 0;
		} else {
			return +1;
		}
	}
}
