public class DoubleCache{ // TODO needs better name
	private final double[] values;
	private boolean filled = false;
	private int currentIndex = -1;

	public DoubleCache(int size) {
		values = new double[size];
	}

	public void addValue(double value){
		currentIndex++;
		if(currentIndex >= values.length){
			currentIndex = 0;
			filled = true;
		}
		values[currentIndex] = value;
	}

	public double sum(){
		double sum = 0;
		for(int i = 0; i < values.length; i++){
			if(!filled && i > currentIndex) break;
			sum += values[i];
		}
		return sum;
	}

	public double min(){
		double lowest = values[0];
		for(int i = 1; i < values.length; i++){
			if(!filled && i > currentIndex) break;
			if(values[i] < lowest) lowest = values[i];
		}
		return lowest;
	}

	public double max(){
		double highest = values[0];
		for(int i = 1; i < values.length; i++){
			if(!filled && i > currentIndex) break;
			if(values[i] > highest) highest = values[i];
		}
		return highest;
	}

	public double mean(){
		return sum() / (filled ? values.length : currentIndex + 1);
	}
}