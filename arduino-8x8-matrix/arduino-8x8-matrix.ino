/*
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	| Piedino della matrice 8x8 | Riga Pilotata (Anodo) | Colonna Pilotata (Catodo) | Pin di connessione per Arduino |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           1               |           5           |                           |               D4               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           2               |           7           |                           |               D11              |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           3               |                       |             2             |               D12              |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           4               |                       |             3             |               D7               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           5               |           8           |                           |               A0 (14)          |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           6               |                       |             5             |               D2               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           7               |           6           |                           |               D10              |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           8               |           3           |                           |               D3               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           9               |           1           |                           |               A3 (17)          |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           10              |                       |             4             |               D13              |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           11              |                       |             6             |               D6               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           12              |           4           |                           |               D8               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           13              |                       |             1             |               D9               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           14              |           2           |                           |               A2 (16)          |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           15              |                       |             7             |               A1 (15)          |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
	|           16              |                       |             8             |               D5               |
	| ------------------------- | --------------------- | ------------------------- | ------------------------------ |
*/

// Global Variables
#define NUMBER_OF_PINS 17
#define MATRIX_SIZE 8
#define CHAR_SIZE 65
#define NUMBER_OF_BEATS 5

// Il valore "99" e' un filler, utilizzato solo per riempire il primo (ed inutile) campo della matrice
int pins[NUMBER_OF_PINS] = {99, 5, 4, 3, 2, A0, A1, A2, A3, 13, 12, 11, 10, 9, 8, 7, 6};

// Nel seguente array viene definita la correlazione tra le colonne (dalla 1 alla 8) e le porte di
// Arduino a cui sono collegate. Guardando la precedente tabella si puo’ osservare che la supportColumn 1 e’
// collegata alla porta definita da pins[13] e cioe’ alla porta 9; la supportColumn 2 e’ collegata al pins[10] e
// cioe’ porta 12, eccetera...
int cols[MATRIX_SIZE] = {pins[13], pins[10], pins[15], pins[9], pins[4], pins[16], pins[6], pins[1]};

// Nel seguente array viene definita la correlazione tra le righe (dalla 1 alla 8) e le porte di
// Arduino a cui sono collegate. Guardando la precedente tabella si puo’ osservare che la riga 1 e’
// collegata alla porta definita dal pins[8] e cioe’ alla porta 17 (A3); la riga 2 e’ collegata al pins[7] e
// cioe’ porta 16 (A2), eccetera...
int rows[MATRIX_SIZE] = {pins[8], pins[7], pins[3], pins[14], pins[2], pins[12], pins[11], pins[5]};

// Utilities
int supportExposureMatrix[CHAR_SIZE]; // area di lavoro utilizzata nella routine di esposizione della matrice
int supportScrollText[CHAR_SIZE]; // area di lavoro utilizzata nella routine di gestione dello scrollmento testo
int supportColumn[MATRIX_SIZE]; // array per la memorizzazione della supportColumn da inviare alla routine di scrollmento testo
int flag = 0; // deviatore utilizzato per fornire il segnale di "showColumn()"

// Matrice della lettera "A" maiuscola
int charA[CHAR_SIZE] = {
	99,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
};

// Matrice della lettera "n" minuscola
int charN[CHAR_SIZE] = {
	99,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 1, 0,
};

// Matrice della lettera "t" minuscola
int charT[CHAR_SIZE] = {
	99,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0,
};

// Matrice della lettera "o" minuscola
int charO[CHAR_SIZE] = {
	99,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
};

// Matrice Contorno del Cuore
int frame[CHAR_SIZE] = {
	99,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	1, 0, 0, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	0, 1, 0, 0, 0, 0, 1, 0,
	0, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
};

// Matrice Cuore
int heart[CHAR_SIZE] = {
	99,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
};


void setup() {
	for (int i = 1; i < NUMBER_OF_PINS; i++) {
		pinMode(pins[i], OUTPUT);
	}
}

void loop() {
	// Spegnimento preventivo di tutti i Led della matrice
	turnOffMatrix();

	// Blocco Accensione e Spegnimento
	{
		// Accende progressivamente tutte le righe
		turnRows(HIGH);

		// Spegne progressivamente tutte le righe
		turnRows(LOW);
	}

	// Blocco Scorrimento Testo
	{
		for (int i = 0; i < MATRIX_SIZE; i++) {
			digitalWrite(rows[i], LOW); // mette in stato di LOW tutte le righe (gli anodi)
			digitalWrite(cols[i], HIGH); // mette in stato di HIGH tutte le colonne (i catodi)

			// init zeros matrix
			initZeros(supportScrollText, CHAR_SIZE);
			initZeros(supportExposureMatrix, CHAR_SIZE);
		}

		// Gestione lettera "A"
		managementScrollText(charA);

		// Gestione lettera "n"
		managementScrollText(charN);

		// Gestione lettera "t"
		managementScrollText(charT);

		// Gestione lettera "o"
		managementScrollText(charO);

		// Gestione della fase di fine scrollmento testo
		initZeros(supportColumn, MATRIX_SIZE);
		for (int i = 0; i < MATRIX_SIZE; i++) {
			showColumn();
		}
	}

	// Blocco Animazione Cuore
	{
		for (int i = 0; i < NUMBER_OF_BEATS; i++) {
			// Routine di visualizzazione del matrixFrame
			managementHeartAnimation(frame);

			// Routine di visualizzazione del cuore
			managementHeartAnimation(heart);
		}
	}
}

void turnOffMatrix(void) {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		digitalWrite(rows[i], LOW); // spegne tutte le righe (anodi)
		digitalWrite(cols[i], LOW); // spegne tutte le colonne (catodi)
	}
}

void turnRows(uint8_t state) {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		digitalWrite(rows[i], state);
		delay(100);
	}
}

void initZeros(int array[], int size) {
	for (int i = 0; i < size; i++) {
		array[i] = 0;
	}
}

void exposure(void) {
	// ripete 300 volte la routine di esposizione (fa il refresh)
	// per ottenere qualche attimo di visibilita', prima di passare alla matrice successiva
	for (int refresh = 0; refresh < 300; refresh++) {
		for (int row = 0; row < MATRIX_SIZE; row++) {
			for (int col = 0; col < MATRIX_SIZE; col++) {
				if (supportExposureMatrix[row * MATRIX_SIZE + (col + 1)] == 1) {
					digitalWrite(rows[row], HIGH);
					digitalWrite(cols[col], LOW);
				}
				digitalWrite(rows[row], LOW);
				digitalWrite(cols[col], HIGH);
			}
		}
	}
}

void showColumn(void) {
	for (int col = 0; col < MATRIX_SIZE - 1; col++) {
		for (int row = 0; row < MATRIX_SIZE; row++) {
			supportExposureMatrix[row * MATRIX_SIZE + (col + 1)] = supportExposureMatrix[row * MATRIX_SIZE + (col + 2)]; // sposta a sinistra ogni colonna
			if (col == (MATRIX_SIZE - 2)) {
				supportExposureMatrix[row * MATRIX_SIZE + (col + 2)] = supportColumn[row]; // inserisce la nuova colonna da visualizzare
			}
		}
	}
	exposure();
}

void scrollText(void) {
	for (int col = 0; col < MATRIX_SIZE; col++) {
		for (int row = 0; row < MATRIX_SIZE; row++) {
			supportColumn[row] = supportScrollText[row * MATRIX_SIZE + (col + 1)];
			if (supportColumn[row] == 1) {
				flag = 1;
			}
		}
		if (flag == 1) {
			showColumn();
			flag = 0;
		}
	}
}

void managementScrollText(int charX[]) {
	for (int i = 0; i < CHAR_SIZE; i++) {
		supportScrollText[i] = charX[i];
	}
	scrollText();
}

void managementHeartAnimation(int array[]) {
	for (int i = 0; i < CHAR_SIZE; i++) {
		supportExposureMatrix[i] = array[i];
	}
	exposure();
}
