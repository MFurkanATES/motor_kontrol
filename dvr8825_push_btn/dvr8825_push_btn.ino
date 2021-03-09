#define pwm_signal_in 1 
#define pwm_signal_in_pin 3 
#define neutral_pwm 0
#define dirPin 8
#define stepPin 9
#define push_btn 6

volatile int n_pwm_in = neutral_pwm; 
volatile unsigned long ul_start_period = 0; 
volatile boolean b_new_pwm_signal = false; 

int delay_time = 10;
int set_speed = 0;
int duration = 0;
int btn_state = 0;
int init_sys = 0;

void setup()
{  
  attachInterrupt(pwm_signal_in,calcInput,CHANGE);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(push_btn, INPUT);
  //Serial.begin(9600);
}



void loop()
{
  if (init_sys == 0){
    first_init();    
  }
  
  if(b_new_pwm_signal)
 {   
    b_new_pwm_signal = false;
 }   
  if(n_pwm_in != 0  ){
    duration = n_pwm_in;
    set_speed = abs(1500 - duration) ;  
    //delay_time = 4000 - (set_speed / 1);
    //delay_time = 4000;
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
    //Serial.println("duration");
    //Serial.println(duration);
   
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
      if(950 < n_pwm_in && n_pwm_in < 2050){
        n_pwm_in = n_pwm_in;
      }
      else{
         n_pwm_in = 0;
      } 
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

void first_init(){
  btn_state = digitalRead(push_btn);
  if(btn_state == HIGH && init_sys ==0){
  //Serial.println("init");
  delay_time = 4000;
  motor_ileri();
  }
  if (btn_state == LOW){
    init_sys = 1;
    //Serial.print("ok");
    for(int x = 0;x<100;x++){
        motor_geri();
        //Serial.println(x);
        //Serial.println("init2");
    }

  }
  
  
}
