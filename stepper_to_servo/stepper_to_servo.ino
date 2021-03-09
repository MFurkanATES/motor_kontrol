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
volatile int duration = 0;
int btn_state = 0;
int init_sys = 0;
int angle = -10;
int set_angle = 0;
int diff_angle = 0;
int test = 0;

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
    //Serial.println("wait");
 }   
  if(n_pwm_in != 0 && init_sys == 1){
    duration = n_pwm_in;    
    delay_time = 3000;
    //Serial.println("-----");   
    //Serial.println(duration);
    //Serial.print(delay_time);
    set_angle = 90 + ((duration - 1500.0) * (90.0/400.0));  
    //test = ((duration - 1500.0) * (90.0/400.0)); 
    //Serial.print("test: ");
    //Serial.println(test);
    diff_angle = set_angle - angle;
    //Serial.print("set_angle");
    //Serial.println(set_angle);
    //Serial.print("diff_angle");
    //Serial.println(diff_angle);
    //Serial.print("angle");
    //Serial.println(angle);
    if (diff_angle > 2 && angle < 181){
      for(int a = 0;a !=diff_angle;a++){
        motor_ccw();
        angle  = angle + 1;
        //Serial.println("motor_ccw");
      } 
     }
    if(diff_angle < -2 && angle > 0){
      test = (-1 * diff_angle) + 1;
      //Serial.print("test");
      //Serial.println(test);
      for(int t = test;t > 1;t--){
        motor_cw();
        angle = angle - 1;
        //Serial.println("motor_cw");
        //Serial.print("t");
        //Serial.println(t);
        //Serial.print("diff_angle");
        //Serial.print(diff_angle);
        //Serial.print("angle");
        //Serial.print(angle);        
      }

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
      if(950 < n_pwm_in && n_pwm_in < 2050){
        n_pwm_in = n_pwm_in;
      }
      else{
         n_pwm_in = 0;
      } 
    }
  }
}

void motor_cw(){
  
digitalWrite(dirPin, LOW);
digitalWrite(stepPin, HIGH);
delayMicroseconds(delay_time);
digitalWrite(stepPin, LOW);
delayMicroseconds(delay_time);
//Serial.println("cw");

}

void motor_ccw(){  

digitalWrite(dirPin, HIGH);
digitalWrite(stepPin, HIGH);
delayMicroseconds(delay_time);
digitalWrite(stepPin, LOW);
delayMicroseconds(delay_time); 
//Serial.println("ccw");
  
}

void first_init(){
  btn_state = digitalRead(push_btn);
  if(btn_state == HIGH && init_sys == 0){
  Serial.println("init");
  delay_time = 4000;
  motor_cw();
  }
  if (btn_state == LOW){
    init_sys = 1;
    angle = -10;
    //Serial.print("ok");
    for(int x = 0;x<100;x++){
        motor_ccw();
        //Serial.println(x);
        Serial.println("init--okey ");
        angle = angle + 1;
    }

  }
  
  
}
