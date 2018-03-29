#define RMF 3       
#define RMB 2      
#define LMB 5      
#define LMF 4
#define LMSpeed 9
#define RMSpeed 10

String a="";  

bool debug=false;
int lmspeed=0,rmspeed=0,bspeed=0;
int topspeed=255,minspeed=0;
int kp=6,kd=8;
float errors[3]={0,0,0};
int dist[3]={0,0,0};
int trigpins[3]={12,8,6};
int echopins[3]={13,11,7};

long ultra(int trig,int echo){
  long dur;
  int distance;
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  dur=pulseIn(echo,HIGH);
  //return 0.01715*dur;
  distance=0.01715*dur;
  if(distance>100) return 100;
  else return distance;
}

void calcerror(int dist[]){
  for(int i=0;i<3;i++){
    (dist[i]<=100)?errors[i]=(100/dist[i]):errors[i]=0;
  }
}

void forward(){
  if(lmspeed>255){
    lmspeed=255;
  }
  else if(lmspeed<0){
    lmspeed=0;
  }
  if(rmspeed>255){
    rmspeed=255;
  }
  else if(rmspeed<0){
    rmspeed=0;
  }
  digitalWrite(LMF, HIGH);
  digitalWrite(LMB, LOW);
  digitalWrite(RMF, HIGH);
  digitalWrite(RMB, LOW);
  analogWrite(LMSpeed, lmspeed);
  analogWrite(RMSpeed, rmspeed);
}

void ultrasonic_pid(int dist[]){
  calcerror(dist);
  //L & R sensors are not detecting and center is open
  if(errors[1]>1){
  //
    lmspeed=0;
    rmspeed=0;
    digitalWrite(LMB, HIGH);
    digitalWrite(RMB, HIGH);
    forward(); 
  }
   else if(errors[0]!=errors[2]){
  //Check for L & R sensor obstacle{
    bspeed=kd*(errors[0]-errors[2]);
    if(debug) Serial.println("bspeed: "+String(bspeed));
    lmspeed=lmspeed+bspeed+kp;
    rmspeed=rmspeed-bspeed+kp;
    digitalWrite(LMB, LOW);
    digitalWrite(RMB, LOW);
    forward();  
  }
  else if((errors[0] == errors[2])&& errors[1]<6){
    digitalWrite(LMB, LOW);
    digitalWrite(RMB, LOW);
    //lmspeed=120;
    //rmspeed=120;
    follow(); 
  }
  else{
    digitalWrite(LMB, HIGH);
    digitalWrite(RMB, HIGH);
    lmspeed=0;
    rmspeed=0;
    follow();
    
  }
  //ultrasonic_pid(dist);
  //forward(); 
}

void ultrasonic_sensors()
{
 for(int i=0;i<3;i++){
    dist[i]=ultra(trigpins[i],echopins[i]);
  } 
}

void setup(){
  for(int i=0;i<3;i++){
    pinMode(trigpins[i],OUTPUT);
    pinMode(echopins[i],INPUT);
  }
  Serial.begin(9600);
}

void follow()
{
 if(Serial.available()){
   long p=Serial.parseInt();
   
   if(p!=5)
   {
   //Serial.println(p);
   if(p<200)
  {
    
    digitalWrite(RMB, HIGH);
    digitalWrite(LMB, LOW);
    //Serial.println("go left");
    lmspeed=50;
    rmspeed=0;
    forward();
    
  }
   else if(p>400)
   {
    digitalWrite(LMB, HIGH);
    digitalWrite(RMB, LOW);
    //Serial.println("go right");
    lmspeed=0;
    rmspeed=50;
    forward();
   }
   else 
   {
    
    digitalWrite(LMB, LOW);
    digitalWrite(RMB, LOW);
    //Serial.println("go forward");
    lmspeed=60;
    rmspeed=60;
    forward();
   }
   }
 }
 else {
  lmspeed=0;
  rmspeed=0;
  analogWrite(LMSpeed, lmspeed);
  analogWrite(RMSpeed, rmspeed);
  return;
 }
}

 

void loop()
{
  
 ultrasonic_sensors();
   ultrasonic_pid(dist);
  //delay(100);
  if(debug){
    delay(1000);
    Serial.println("Distances: "+String(dist[0])+"\t"+String(dist[2]));
    Serial.println("Left Error: "+String(errors[0])+"\tRight Error: "+String(errors[2]));
    Serial.println("LMspeed: "+String(lmspeed)+"\tRMspeed: "+String(rmspeed));
  }
}


