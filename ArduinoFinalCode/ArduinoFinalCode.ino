//#define RMF 3       
//#define RMB 2      
//#define LMB 5      
//#define LMF 4
//#define LMSpeed 9
//#define RMSpeed 10

#define RFD 7
#define RFB 27
#define RFS 5

#define LFD 24
#define LFB 25
#define LFS 3

#define RBD 6
#define RBB 29
#define RBS 4

#define LBD 22
#define LBB 23
#define LBS 2

int rfspeed=0;
int rbspeed=0;
int lfspeed=0;
int lbspeed=0;

String a="";  

bool debug=true;
int lmspeed=0,rmspeed=0,bspeed=0;
int topspeed=255,minspeed=0;
int kp=6,kd=8;
float errors[3]={0,0,0};
int dist[3]={0,0,0};
int trigpins[3]={34,32,30};
int echopins[3]={35,33,31};



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
  Serial.println("left= "+String(dist[0]) + " center="+ String(dist[1]) + " right= "+String(dist[2]));
  for(int i=0;i<3;i++){
    (dist[i]<=100)?errors[i]=(100/dist[i]):errors[i]=0;
  }
}

void checkbound()
{
  if(lfspeed>255){
    //lmspeed=255;
    lfspeed = 255;
    lbspeed = 255;
  }
  else if(lfspeed<0){
    lfspeed = 0;
    lbspeed = 0;
//    digitalWrite(LFB, HIGH);
//    digitalWrite(LBB, HIGH);
  }
  if(rfspeed>255){
    rfspeed = 255;
    rbspeed = 255;
  }
  else if(rfspeed<0){
    rfspeed = 0;
    rbspeed = 0;
//    digitalWrite(RFB, HIGH);
//    digitalWrite(RBB, HIGH);
  }
}

void forward(){
  
//  digitalWrite(LMF, HIGH);
//  digitalWrite(LMB, LOW);
//  digitalWrite(RMF, HIGH);
//  digitalWrite(RMB, LOW);
//  analogWrite(LMSpeed, lmspeed);
//  analogWrite(RMSpeed, rmspeed);

  checkbound();
  digitalWrite(RFD, HIGH);
  digitalWrite(RFB, LOW);
  digitalWrite(RBD, HIGH);
  digitalWrite(RBB, LOW);
  digitalWrite(LFD, HIGH);
  digitalWrite(LFB, LOW);
  digitalWrite(LBD, HIGH);
  digitalWrite(LBB, LOW);
  analogWrite(RFS,rfspeed);
  analogWrite(RBS,rbspeed);
  analogWrite(LFS,lfspeed);
  analogWrite(LBS,lbspeed);
}

void ultrasonic_pid(int dist[]){
  calcerror(dist);
  //L & R sensors are not detecting and center is open
  if(errors[1]>1){
  //
//    lfspeed=0;
//    rfspeed=0;
//    lbspeed=0;
//    rbspeed=0;
    digitalWrite(LFB, HIGH);
    digitalWrite(RFB, HIGH);
    digitalWrite(LBB, HIGH);
    digitalWrite(RBB, HIGH);
    //forward(); 
  }
   else if(errors[0]!=errors[2]){
  //Check for L & R sensor obstacle{
    bspeed=kd*(errors[0]-errors[2]);
    if(debug) Serial.println("bspeed: "+String(bspeed));
    lfspeed=lfspeed+bspeed+kp;
    lbspeed=lbspeed+bspeed+kp;
    rfspeed=rfspeed-bspeed+kp;
    rbspeed=rbspeed-bspeed+kp;
    checkbound();
    digitalWrite(LFB, LOW);
    digitalWrite(RFB, LOW);
    digitalWrite(LBB, LOW);
    digitalWrite(RBB, LOW);
    forward();  
  }
  else if((errors[0] == errors[2])&& errors[1]<6){
  
    digitalWrite(LFB, LOW);
    digitalWrite(RFB, LOW);
    digitalWrite(LBB, LOW);
    digitalWrite(RBB, LOW);
    //lmspeed=120;
    //rmspeed=120;
    follow(); 
  }
  else{
    digitalWrite(LFB, HIGH);
    digitalWrite(RFB, HIGH);
    digitalWrite(LBB, HIGH);
    digitalWrite(RBB, HIGH);
    lfspeed=0;
    rfspeed=0;
    lbspeed=0;
    rbspeed=0;
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
  pinMode(RFD,OUTPUT);
pinMode(RFB,OUTPUT);
pinMode(RBD,OUTPUT);
pinMode(RBB,OUTPUT);
pinMode(LFB,OUTPUT);
pinMode(LFD,OUTPUT);
pinMode(LBD,OUTPUT);
pinMode(LBB,OUTPUT);
pinMode(RBS,OUTPUT);
pinMode(LBS,OUTPUT);
pinMode(LFS,OUTPUT);
pinMode(RFS, OUTPUT);  
  Serial.begin(9600);
}

void follow()
{
 if(Serial.available()){
   long p=Serial.parseInt();

  //p=300;
   
   if(p!=5)
   {
   //Serial.println(p);
   if(p<200)
  {
//    digitalWrite(RMB, HIGH);
//    digitalWrite(LMB, LOW);
    //Serial.println("go left");
    lfspeed=50;
    lbspeed=50;
    rfspeed=0;
    rbspeed=0;
    forward(); 
  }
   else if(p>400)
   {
//    digitalWrite(LMB, HIGH);
//    digitalWrite(RMB, LOW);
    //Serial.println("go right");
    lfspeed=0;
    lbspeed=0;
    rfspeed=50;
    rbspeed=50;
    forward();
   }
   else 
   {
    
//    digitalWrite(LMB, LOW);
//    digitalWrite(RMB, LOW);
    //Serial.println("go forward");
    lfspeed=60;
    lbspeed=60;
    rfspeed=60;
    rbspeed=60;
    forward();
   }
   }
 }
 else {
  lfspeed=0;
  lbspeed=0;
  rfspeed=0;
  rbspeed=0;
//  analogWrite(LMSpeed, lmspeed);
//  analogWrite(RMSpeed, rmspeed);
  return;
 }
}

 

void loop()
{
 ultrasonic_sensors();
   ultrasonic_pid(dist);
  //delay(100);
  if(debug){
    //delay(1000);
    Serial.println("Distances: "+String(dist[0])+"\t"+String(dist[2]));
    Serial.println("Left Error: "+String(errors[0])+"\tRight Error: "+String(errors[2]));
    Serial.println("LMspeed: "+String(lfspeed)+"\tRMspeed: "+String(rfspeed));
  }
}


