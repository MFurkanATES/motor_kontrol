#define pwm_signal_in 1 
#define pwm_signal_in_pin 3 
#define neutral_pwm 1500 
#define dirPin 6
#define stepPin 7

volatile int n_pwm_in = neutral_pwm; 
volatile unsigned long ul_start_period = 0; 
volatile boolean b_new_pwm_signal = false; 

int delay_time = 10;
int set_speed = 0;
int duration = 0;

void setup()
{  
  attachInterrupt(pwm_signal_in,calcInput,CHANGE);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600);
}



void loop()
{
  if(b_new_pwm_signal)
 {
   if(950 < n_pwm_in && n_pwm_in < 2050){
    n_pwm_in = n_pwm_in;
   }
   else{
    n_pwm_in = 0;
   } 
   //Serial.println(n_pwm_in);
   b_new_pwm_signal = false;
   }   
   if(n_pwm_in != 0){
   duration = n_pwm_in;
   set_speed = abs(1500 - duration) ;  
   delay_time = 200 - (set_speed / 10);
   //Serial.println("-----");   
   //Serial.println(duration);
   //Serial.print(delay_time);
   if(duration >= 1512 ){    
     motor_geri();
     //Serial.println(delay_time);
     //Serial.println(duration);
   }

   if(1489 >= duration){       
        motor_ileri();        
   }
   
   }
 }

 // other processing ...


void calcInput()
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(pwm_signal_in_pin) == HIGH)
  {

    ul_start_period = micros();
  }
  else
  {
    
    if(ul_start_period && (b_new_pwm_signal == false))
    {
      n_pwm_in = (int)(micros() - ul_start_period);
      ul_start_period = 0;      
      b_new_pwm_signal = true;
    }
  }
}

void motor_ileri(){
  
digitalWrite(dirPin, LOW);
digitalWrite(stepPin, HIGH);
delayMicroseconds(delay_time);
digitalWrite(stepPin, LOW);
delayMicroseconds(delay_time);
//Serial.println("ileri");

}

void motor_geri(){  

digitalWrite(dirPin, HIGH);
digitalWrite(stepPin, HIGH);
delayMicroseconds(delay_time);
digitalWrite(stepPin, LOW);
delayMicroseconds(delay_time); 
//Serial.println("geri");
  
}
