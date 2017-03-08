package edu.osu.hudl.common;

import java.util.Vector;


public interface IClassifier {
	public void learn(Vector<IExample> examples);

	public ClassificationResult predict(Vector<IExample> examples, boolean flag);

	public ClassificationResult predict(Vector<IExample> examples);
	
	public ClassificationResult predict2(Vector<IExample> examples);
}
