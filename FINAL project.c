typedef struct _lled{
  int stat;
  int cul;
  int row;
} lled;

lled SCR[32];
int life = 10; 
int score = 0;
int P = 28;
int data[4];
char num[] = { 126, 48, 107, 121, 53, 93, 95, 112, 127 , 125};
int maxfreq = 0;


//********************************************
/*8*8 led screen*/
void showlled( int L);
void shutlled( int L);
void pinsforSCR();
void SetLED();
void rollingdown();
void checkroll();
void showSCR( );


/* Game ralating*/
void Movepos( int D);
void Gameover();

/* 4*7 segenment LED*/
void pinsfor7x4();
void displaynum( char a);
void clearnum();
void testled();
void displaylife();
void displayscore();

/* to deal voice signal*/
int getonefreq();
int Getdata();
   


//*********************************************


void setup() {
  // put your setup code here, to run once:
 // Serial.begin(9600);
  pinMode( A0, INPUT);
  pinMode( A1, OUTPUT);
  pinMode( A4, OUTPUT);
  pinMode( A5, OUTPUT);
  pinMode( A2, INPUT);
  Serial.begin(9600);
  pinsforSCR();
  /*set up the pins*/
  SetLED();
  /*set up the 8*8 led */
  pinsforSCR();
  testled();
 /*code below is to control*/

}

void loop() {
   static int ppos = analogRead(A0 );
   static int index = 0;
   static int turns = 0;
   int npos = analogRead( A0);
   Movepos( npos - ppos);
   ppos = npos;
   if( turns % 8 == 0) {
     
     
     int nextled = Getdata();
     if( nextled != 0)  SCR[nextled - 1].stat = 1;
     else Serial.print("it's zero!");
     rollingdown(); 
   }
   

   //display
/*   for( int j = 0; j < 5; j++) {
    pinsforSCR();
    showSCR();
   }
*/
  
  
  turns++;

}


//*************************************************
//*************************************************

void showlled( int L){
    
    digitalWrite( SCR[L].cul + 2, HIGH);
    digitalWrite( SCR[L].row + 10, LOW );
  
}

void shutlled( int L){
  digitalWrite( SCR[L].cul + 2, LOW );
  digitalWrite( SCR[L].row + 10, HIGH);;
  
}

void pinsforSCR(){
  for( int i = 0; i < 12; i++){
    pinMode( i+2, OUTPUT);
  }

  for( int i = 0; i < 8; i++){
    digitalWrite( i + 2, LOW );
  }
  for( int i = 0; i < 4; i++){
    digitalWrite( i + 10, HIGH);
  }
  digitalWrite( A1, HIGH);
  digitalWrite( A4, HIGH);
  digitalWrite( A5, HIGH);
}

void SetLED(){
   int cnt = 0;
   for( int i = 0; i < 8; i++){
      for( int j = 0; j < 4; j++){
        SCR[cnt].stat = 0;
        SCR[cnt].cul  = i;
        SCR[cnt].row  = j;
        cnt++;
      }
  }
  
  for( int i = 0; i < 32; i++){
     SCR[i].stat = 1;
     for( int j = 0; j < 10; j++) showSCR();
     SCR[i].stat = 0;
  }
  checkroll();
}


void rollingdown( ){
  for( int i = 27; i >= 0; i--){
  /*why change the position it will be normal?*/  
    if( SCR[i].stat == 1){
      SCR[i].stat = 0;
      if( i + 4 <= 27) {
      SCR[i+4].stat = 1;
      }
      else if( SCR[i+4].stat == 1) score++;
      else life--;
    }
  }
  /*insert score here*/
}

void checkroll(void){
  SCR[1].stat = 1;
  SCR[4].stat = 1;
  SCR[10].stat = 1;
  SCR[15].stat = 1;
  SCR[P].stat = 1;
  for( int i = 0; i < 8; i++){
   for( int j = 0; j < 5; j++) showSCR();
   rollingdown();
  }
  
}

void showSCR( ){
  pinsforSCR();
  for( int i = 31; i >= 0; i--){
  /*why change the position it will be normal?*/  
    if( SCR[i].stat == 1){
      showlled( i);
      delay(1);
      shutlled( i);
    }
  }
  
  pinsfor7x4();
  displaylife();
  delay(1);
  
  displayscore();
  delay(1);
  
}

void Movepos( int D){
  SCR[P].stat = 0;
//  Serial.println( D);
  if ( abs( D) > 5) {
    if ( D > 0) P++;
    if ( D < 0) P--;
  }
  
  if( P < 28) P = 28;
  if( P > 31) P = 31;
  SCR[P].stat = 1;
}



void displaynum( char a){
  
  for( int i = 1; i < 8; i++){
    if( (0x80 & a<<i) == 0x80) digitalWrite( i+1,HIGH);
    else digitalWrite( i+1, LOW); 
  }
  
}

void clearnum(){
  
  for( int i = 2; i <= 8; i++){
    digitalWrite( i, LOW);
  }
  digitalWrite( A4, HIGH);
  digitalWrite( A5, HIGH);
  digitalWrite( A1, HIGH);
  
 }

void testled(){
  
  for( int i = 0; i < 20; i++){
    clearnum();
    digitalWrite( 0, LOW);
    digitalWrite( 1, LOW);
    digitalWrite( A1, LOW);
    delay(50);
  }
  
 }

void displaylife( ){
  digitalWrite( A1, LOW);
  displaynum( num[life]);
  clearnum();  
 }

 void displayscore( ){
  digitalWrite( A4, LOW);  
  displaynum( num[score/10]);
  clearnum();
  digitalWrite( A5, LOW);  
  displaynum(num[score%10]);
  clearnum(); 
 }

 void pinsfor7x4(){
  for( int i = 2; i < 10; i++){
    pinMode( i, OUTPUT);
  }

  for( int i = 0; i < 7; i++){
    digitalWrite( i + 2, LOW );
  }
  
    digitalWrite( A5, HIGH);
    digitalWrite( A4, HIGH);
    digitalWrite( A1,LOW);
  
}

int getonefreq(){
  
  int predata = analogRead( A2);
  int thisdata = analogRead( A2);
  int nextdata = 0;
  int d1 = 0;
  int d2 = 0;
  int freq = 0;
  
  for( int i = 0; i < 64; i++){
    nextdata = analogRead( A2);
    d1 = thisdata - predata;
    d2 = nextdata - thisdata;
    
    if( ( abs(d1) > 3) && ( abs(d2) > 3)){
      if( (d1 > 0 && d2 < 0) || (d2 > 0 && d1 < 0))
      freq++;
    }
    
    predata  = thisdata;
    thisdata = nextdata;

  /*code below to 防止闪屏*/
  if( i % 16 == 0){
    pinsforSCR();
    showSCR();
  }
  

  }
 
  return freq;
}

int Getdata(){
   int sum = 0;
   double rate = 0.0;
   for ( int i = 0; i < 4; i++){
    int temp = getonefreq();
    if( temp != 0) {
      data[i] = temp;
      sum += temp;
      if( temp > maxfreq) maxfreq = temp;
    }
  }
  rate = (double)( sum / 4.0 ) / (double)maxfreq;
  if( rate > 0.4) return 4;
  else if( rate > 0.25) return 3;
  else if( rate > 0.15) return 2;
  else if( rate > 0.05) return 1;
  else return 0;
  
}

