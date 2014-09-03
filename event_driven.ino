// Sistemas Reativos.ino
#define LED_PIN 7
#define ABUT_PIN 9
#define DBUT_PIN 11
#define STOP_TIME 500

int state =0;
unsigned long timer_L, timer_BA, timer_BD;
int A_button, D_button, last_checkA, last_checkD;
unsigned pisca = 1000;
unsigned long now;


void button_listen (int pin)
{
	if(pin == ABUT_PIN)
	{
		A_button = 1;
	}
	else
	{
		D_button = 1;
	}

}

void timer_set (int pin)
{
	if(pin == LED_PIN){
		timer_L = millis();
	}
	else if(pin == ABUT_PIN)
	{
		timer_BA = millis();
	}
	else if(pin == DBUT_PIN)
	{
		timer_BD = millis();
	}
}


void timer_expired(void)
{
	state = !state;
	digitalWrite(LED_PIN, state);
}


void stop (void)
{
	digitalWrite(LED_PIN, LOW);
	exit(1);
}


void button_changed (int pin)
{

	if(pin == ABUT_PIN){
		if(now - timer_BD < STOP_TIME)
			stop();

		pisca -= 250;
	}
	else
	{
		if(now - timer_BA < STOP_TIME)
			stop();
		pisca += 250;
	}
}


void setup() {
	pinMode(LED_PIN, OUTPUT);
	pinMode(ABUT_PIN, INPUT);
	pinMode(DBUT_PIN, INPUT);

    timer_set(LED_PIN);
    timer_set(ABUT_PIN);
    timer_set(DBUT_PIN);
    button_listen(ABUT_PIN);
    button_listen(DBUT_PIN);

    last_checkA = HIGH;
    last_checkD = HIGH;
    timer_BA = 0;
    timer_BD = 0;
}

void loop() {
	int butA, butD;
// se encarrega de fazer o LED piscar
	now = millis();
	if(now - timer_L > pisca)
	{
		timer_expired();
		timer_set(LED_PIN);
	}

// acelerando o pisca
	if(A_button)
	{
		butA = digitalRead(ABUT_PIN);
			
		if(last_checkA == butA)
		{	
			last_checkA = HIGH;

			if(butA)
			{
				last_checkA = LOW;
				button_changed(ABUT_PIN);
				timer_set(ABUT_PIN);
			}
		}
	}

// desacelera
	if(D_button)
	{
		butD = digitalRead(DBUT_PIN);

		if(last_checkD == butD)
		{	
			last_checkD = HIGH;

			if(butD)
			{
				last_checkD = LOW;
				button_changed(DBUT_PIN);
				timer_set(DBUT_PIN);
			}
		}
	}

}

