void automataStep(unsigned char* lifeBoard, const int width, const int height, const bool toroid){

	int rowFirst = 0; //Representa al primer indice de la fila actual

	char* bufferAbove; //fila superior a la actual en el estado anterior
	bufferAbove = new unsigned char[width];

	char* bufferCurrent; //copia de la fila actual en el estado anterior para luego pasar a ser bufferAbove
	bufferCurrent = new unsigned char[width];

	int iterator, lifeCount;

	const int boardOverflow = width * height;

	char* bufferTop; //primer fila de la matriz en el estado anterior, necesaria para el mundo toroidal

	if(toroid){ //Si es un toroide, tengo que guardar la fila superior y perparar la ultima como "superior"
		bufferTop = new unsigned char[width];
		for(iterator = 0; iterator < width; ++iterator){
			bufferTop[iterator] = lifeBoard[rowFirst + iterator];
		}
		rowFirst = (height - 1) * width;
		for(iterator = 0; iterator < width; ++iterator){
			bufferAbove[iterator] = lifeBoard[rowFirst + iterator];
		}
	}
	else{
		//Si no es toroidal, debo tomar como superior una fila de 0
		for(iterator = 0; iterator < width; ++iterator){
			bufferAbove[iterator] = 0;
		}		
	}

	//Aplicacion del automata
	for(rowFirst = 0 ; rowFirst < boardOverflow ; rowFirst +=width){
		//Relleno el buffer de fila actual		
		for(iterator = 0 ; iterator < width ; ++iterator){
			bufferCurrent[iterator] = lifeBoard[rowFirst + iterator];
		}
		
		for(iterator = 0 ; iterator < width ; ++iterator){
			/* ---*---Calculo De Vida---*--- */

			lifeCount = 0; //reseteo el contador

			//Si es el primer item de la fila, sumo 0 o el valor de la otra punta si es toroidal
			if(iterator == 0){
				lifeCount += (toroid ? (bufferCurrent[width-1] != 0) : 0); //revisa el punto a su izquierda
				lifeCount += (toroid ? (bufferAbove[width-1] != 0) : 0); //revisa el punto diagonal superior
	//			if(rowFirst + width < boardOverflow ){ //Si tengo una lina debajo
			}
			else{ lifeCount += (bufferCurrent[iterator-1] != 0) + (bufferAbove[iterator-1] != 0); }

			//Si es el ultimo item de la fila
			if(iterator == width-1){
				lifeCount += (toroid ? (bufferCurrent[0] != 0) : 0); //revisa el punto a su izquierda
				lifeCount += (toroid ? (bufferAbove[0] != 0) : 0); //revisa el punto diagonal superior			
			}

			//Si es la ultima fila
			if((rowFirst + width) == boardOverflow){
				if(toroid){
					lifeCount += (bufferTop[iterator] != 0);
					lifeCount += (iterator == width-1 ? bufferTop[0] != 0 : bufferTop[iterator+1] != 0);
					lifeCount += (iterator == 0 ? bufferTop[width-1] != 0 : bufferTop[iterator-1] != 0);
				}
			}
			else{
				if(toroid){
					lifeCount += (lifeBoard[rowFirst+width+iterator] != 0);
					lifeCount += (iterator == width-1 ? lifeBoard[rowFirst+width] != 0 : lifeBoard[rowFirst+width+iterator+1] != 0);
					lifeCount += (iterator == 0 ? lifeBoard[rowFirst+width+width-1] != 0 : lifeBoard[rowFirst+width+iterator-1] != 0);
				}
			}
			/* ---*---Fin Calculo De Vida---*--- */

			if(bufferCurrent[iterator] == 0){ bufferCurrent[iterator] = lifeCount == 3; }
			else{
				if(lifeCount == 2 || lifeCount == 3){
					++lifeBoard[rowFirst + iterator];
					if(lifeBoard[rowFirst + iterator]==0){lifeBoard[rowFirst + iterator]=255;}
				}
				else{lifeBoard[rowFirst + iterator]=0;}
			}
		}
		//Paso de fila: swappeo buffers y paso el inicio de fila a la siguiente
		bufferAbove = bufferAbove xor bufferCurrent;
		bufferCurrent = bufferAbove xor bufferCurrent;
		bufferAbove = bufferAbove xor bufferCurrent;
		rowFirst += width;
	}
}
