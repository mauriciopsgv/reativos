//Arduino Tarefa 02

//Definição das Portas usadas
#define LED 7
#define A_BUT 9
#define D_BUT 11


int state = 1;

int tempo_pisca = 1000;
unsigned long old = 0;		//Ultima vez que o estado do LED foi alterado
unsigned long now;			//Tempo atual
unsigned long both=0;		//Última vez em que um botão foi apertado
int leitura;

void setup() {
	Serial.begin(9600);
	pinMode(LED, OUTPUT);
	pinMode(A_BUT, INPUT);
	pinMode(D_BUT, INPUT);
}

void loop() {
	now = millis();


	// Regulariza o pisca-pisca
	if(now >= old + tempo_pisca)
	{
		old = now;
		state = !state;
		digitalWrite(LED, state);
	}

	// Aumenta a velocidade e checa se deve parar o LED
	
	leitura = digitalRead(A_BUT);
	Serial.print("Ac=");
	Serial.println(leitura);
	if(leitura)
	{
		if(both-now <= 500)
		{
			para_LED();
		}

		Serial.println("ACELERANDO");
		both = now;
		tempo_pisca -= 250;
	}

	// Diminui a velocidade e checa se deve parar o LED
	leitura = digitalRead(D_BUT);
	Serial.print("De=");
	Serial.println(leitura);
	if(leitura)
	{
		if(both-now <= 500)
		{
			para_LED();
		}

		Serial.println("DESACELERANDO");
		both = now;
		tempo_pisca += 250;
	}
}

// Função para parar o LED e a atividade
void para_LED () {
	digitalWrite(LED, LOW);
	exit(1);
}