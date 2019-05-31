
#define RFD 9
#define RFB 7
#define RFS 8

#define LFD 24
#define LFB 25
#define LFS 3

#define RBD 6
#define RBB 4
#define RBS 5

#define LBD 22
#define LBB 23
#define LBS 2


// 7,10 pwm
// 11,8 dir
// 6,9  break

/*
*******
Motors Left
*******
TYPE FRONT REAR
PWM  3     2
DIR  24    22
BRK  25    23


*/


/*
*******
Motors Right
*******

TYPE FRONT REAR
PWM  8    5
DIR  9    6
BRK  7    4


*/



int rfspeed=0;
int rbspeed=0;
int lfspeed=0;
int lbspeed=0;
int adjfactor=0;

String a="";  

bool debug=false;
int bspeed=0;
int topspeed=255,minspeed=0;
int kp=6,kd=8;
float errors[3]={0,0,0};
int dist[3]={0,0,0};
int trigpins[3]={12,8,6};
int echopins[3]={13,11,7};




void setup() {
  // put your setup code here, to run once:

pinMode(6,OUTPUT);
pinMode(7,OUTPUT);
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);

pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(22,OUTPUT);
pinMode(23,OUTPUT);
pinMode(24,OUTPUT);
pinMode(25,OUTPUT);
Serial.begin(9600);
}




void forward()
{
//  rfspeed=255;
// rbspeed=255;
// lfspeed=255;
// lbspeed=255;

 rangeCheck();

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


void backward()
{
  
//   rfspeed=255;
// rbspeed=255;
// lfspeed=255;
// lbspeed=255;


 rangeCheck();

  digitalWrite(RFD, LOW);
  digitalWrite(RFB, LOW);
  digitalWrite(RBD, LOW);
  digitalWrite(RBB, LOW);
  digitalWrite(LFD, LOW);
  digitalWrite(LFB, LOW);
  digitalWrite(LBD, LOW);
  digitalWrite(LBB, LOW);  
   analogWrite(RFS,rfspeed);
 analogWrite(RBS,rbspeed);
 analogWrite(LFS,lfspeed);
 analogWrite(LBS,lbspeed);

}



void turnRight()
{
//     rfspeed=0;
// rbspeed=0;
// lfspeed=255;
// lbspeed=255;

 rangeCheck();
  analogWrite(RFS,rfspeed);
 analogWrite(RBS,rbspeed);
 analogWrite(LFS,lfspeed);
 analogWrite(LBS,lbspeed);

  digitalWrite(RFD, LOW);
  digitalWrite(RFB, LOW);
  digitalWrite(RBD, LOW);
  digitalWrite(RBB, LOW);
  digitalWrite(LFD, HIGH);
  digitalWrite(LFB, LOW);
  digitalWrite(LBD, HIGH);
  digitalWrite(LBB, LOW);  
}

void turnLeft()
{

   rangeCheck();
   
//   rfspeed=255;
// rbspeed=255;
// lfspeed=0;
// lbspeed=0;
  analogWrite(RFS,rfspeed);
 analogWrite(RBS,rbspeed);
 analogWrite(LFS,lfspeed);
 analogWrite(LBS,lbspeed);

  digitalWrite(RFD, HIGH);
  digitalWrite(RFB, LOW);
  digitalWrite(RBD, HIGH);
  digitalWrite(RBB, LOW);
  digitalWrite(LFD, LOW);
  digitalWrite(LFB, LOW);
  digitalWrite(LBD, LOW);
  digitalWrite(LBB, LOW);    
}


void sideRight()
{

 rangeCheck();
  
//     rfspeed=255;
// rbspeed=255;
// lfspeed=255;
// lbspeed=255;
  analogWrite(RFS,rfspeed);
 analogWrite(RBS,rbspeed);
 analogWrite(LFS,lfspeed);
 analogWrite(LBS,lbspeed);
  digitalWrite(RFD, LOW);
  digitalWrite(RFB, LOW);
  digitalWrite(RBD, HIGH);
  digitalWrite(RBB, LOW);
  digitalWrite(LFD, HIGH);
  digitalWrite(LFB, LOW);
  digitalWrite(LBD, LOW);
  digitalWrite(LBB, LOW);  




}


void sideLeft()
{

   rangeCheck();
   
     rfspeed=255;
 rbspeed=255;
 lfspeed=255;
 lbspeed=255;
  analogWrite(RFS,rfspeed);
 analogWrite(RBS,rbspeed);
 analogWrite(LFS,lfspeed);
 analogWrite(LBS,lbspeed);

  digitalWrite(RFD, HIGH);
  digitalWrite(RFB, LOW);
  digitalWrite(RBD, LOW);
  digitalWrite(RBB, LOW);
  digitalWrite(LFD, LOW);
  digitalWrite(LFB, LOW);
  digitalWrite(LBD, HIGH);
  digitalWrite(LBB, LOW);  
}


void diagonalRF()
{

 rangeCheck();
  
//   rfspeed=0;
// rbspeed=255;
// lfspeed=255;
// lbspeed=0;
  analogWrite(RFS,rfspeed);
 analogWrite(RBS,rbspeed);
 analogWrite(LFS,lfspeed);
 analogWrite(LBS,lbspeed);

  digitalWrite(RFD, LOW);
  digitalWrite(RFB, HIGH);
  digitalWrite(RBD, HIGH);
  digitalWrite(RBB, LOW);
  digitalWrite(LFD, HIGH);
  digitalWrite(LFB, LOW);
  digitalWrite(LBD, LOW);
  digitalWrite(LBB, HIGH);    
}


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


void ultrasonic_sensors()
{
 for(int i=0;i<3;i++){
    dist[i]=ultra(trigpins[i],echopins[i]);
  } 
}






void calcerror(int dist[]){
  for(int i=0;i<3;i++){
    (dist[i]<=100)?errors[i]=(100/dist[i]):errors[i]=0;
  }
}

void rangeCheck()
{

  if(lfspeed>255){
    lfspeed=255;
  }
  else if(lfspeed<0){
    lfspeed=0;
  }
  if(rfspeed>255){
    rfspeed=255;
  }
  else if(rfspeed<0){
    rfspeed=0;
  }
  if(rbspeed>255){
    rbspeed=255;
  }
  else if(rbspeed<0){
    rbspeed=0;
  }
  if(lbspeed>255){
    lbspeed=255;
  }
  else if(lbspeed<0){
    lbspeed=0;
  }

  
}




void ultrasonic_pid(int dist[]){
  calcerror(dist);
  //L & R sensors are not detecting and center is open
  if(errors[1]>1){
  //
    lfspeed=255;
    rfspeed=255;
    lbspeed=255;
    rbspeed=255;
    forward(); 
  }
   else if(errors[0]!=errors[2]){
  //Check for L & R sensor obstacle{
    bspeed=kd*(errors[0]-errors[2]);
    if(debug) Serial.println("bspeed: "+String(bspeed));
    lfspeed=lfspeed+bspeed+kp;
    lbspeed=lbspeed+bspeed+kp;
    rfspeed=rmspeed-bspeed+kp;
    rbspeed=rmspeed-bspeed+kp;
    digitalWrite(LFB, HIGH);
    digitalWrite(RFB, HIGH);
    digitalWrite(LBB, HIGH);
    digitalWrite(RBB, HIGH);
    forward();  
  }
  else if((errors[0] == errors[2])&& errors[1]<6){
   
    digitalWrite(LFB, HIGH);
    digitalWrite(RFB, HIGH);
    digitalWrite(LBB, HIGH);
    digitalWrite(RBB, HIGH);
    //lmspeed=120;
    //rmspeed=120;
    follow(); 
  }
  else{

    digitalWrite(LFB, HIGH);
    digitalWrite(RFB, HIGH);
    digitalWrite(LBB, HIGH);
    digitalWrite(RBB, HIGH);
    lmspeed=0;
    rmspeed=0;
    follow();
    
  }
  //ultrasonic_pid(dist);
  //forward(); 
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





void loop() {
  // put your main code here, to run repeatedly:


 ultrasonic_sensors();
 ultrasonic_pid(dist);
 

  
/*





***************

*/


}





