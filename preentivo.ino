//timer interrupts
//timer setup for timer1.
//#define ledPin 13

typedef struct {
  /* period in ticks */
  int period;
  /* ticks until next activation */
  int delay;
  /* function pointer */
  void (*func)(void);
  /* activation counter */
  int exec;
} Sched_Task_t;

Sched_Task_t Tasks[20];
int cur_task = 20;

void Sched_Schedule(void){
  int x;
  for(x=0; x<20; x++) {
    if((Tasks[x].func) && (Tasks[x].delay)){
      Tasks[x].delay--;
      if(!Tasks[x].delay){
        /* Schedule Task */
        Tasks[x].exec++;
        Tasks[x].delay = Tasks[x].period;
      }
    }
  }
}

int Sched_Dispatch(void){
  int prev_task = cur_task;
  int x;
  for(x=0; x<20; x++) {
    if((Tasks[x].func) && (Tasks[x].exec)) {
      Tasks[x].exec--;
      cur_task = x;
      interrupts();
      Tasks[x].func();
      noInterrupts();
      cur_task = prev_task;
      // Delete task if one-shot
      if(!Tasks[x].period)
        Tasks[x].func = 0;
      return 0;
    }
  }
}

void int_handler(void){
  noInterrupts();
  Sched_Schedule();
  Sched_Dispatch();
  interrupts();
};

int Sched_Init(void){
  int x;
  for(x=0; x<20; x++){
    Tasks[x].func = 0;
  }

  //Configure interrupts
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
 
  //OCR1A = 31250; // compare match register 16MHz/256/2Hz
  OCR1A = 31;    // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts

  int_handler();
}

int Sched_AddT(void (*f)(void),int d, int p){
  int x;
  for(x=0; x<20; x++)
    if (!Tasks[x].func) {
      Tasks[x].period = p;
      Tasks[x].delay = d;
      Tasks[x].exec = 0;
      Tasks[x].func = f;
      return x;
    }
  return -1;
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt
  int_handler();
}

void FuncX(void){
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  //digitalWrite(11, HIGH);
}

void FuncY(void){
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
}

void setup(){
  
  pinMode(11, OUTPUT); 
  pinMode(12, OUTPUT);  
  Sched_Init();
  Sched_AddT(FuncX, 1, 2000);
  Sched_AddT(FuncY, 2, 20);
  
}//end setup
  
void loop(){
  //do other things here
  //Sched_Dispatch();
}
