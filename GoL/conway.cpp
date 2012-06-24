public void automataStep(unsigned char* lifeBoard, const int width, const int height, const bool toroid){

	int rowFirst = 0; //Representa al primer indice de la fila actual

	unsigned char* bufferAbove; //fila superior a la actual en el estado anterior
	bufferAbove = new unsigned char[width];

	unsigned char* bufferCurrent; //copia de la fila actual en el estado anterior para luego pasar a ser bufferAbove
	bufferAbove = new unsigned char[width];

	unsigned char bufferTop[]; //primer fila de la matriz en el estado anterior, necesaria para el mundo toroidal
	if(toroid){ //Si es un toroide, tengo que guardar la fila superior y perparar la ultima como "superior"
		bufferTop = new unsigned char[width];
		int iterator;
		for(iterator = 0; iterator < width; ++iterator){
			bufferTop[iterator] = lifeBoard[rowFirst + iterator];
		}
		rowFirst = (height - 1) * width;
		for(iterator = 0; iterator < width; ++iterator){
			bufferAbove[iterator] = lifeBoard[rowFirst + iterator];
		}
		rowFirst = 0;
	}
	else{
		//Si no es toroidal, debo tomar como superior una fila de 0
		for(iterator = 0; iterator < width; ++iterator){
			bufferAbove[iterator] = 0;
		}		
	}
}
