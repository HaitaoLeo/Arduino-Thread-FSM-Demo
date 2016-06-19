// Thread Demo Sketch

// Uses the ArduinoThread library
// by Ivan Seidel
// https://github.com/ivanseidel/ArduinoThread

#include <ThreadController.h>
#include <Thread.h>
#include <TimeLib.h>
#include <Time.h>

#define START   0
#define RESET   1
#define RUNNING 2

// yes, we're using globals. shameful, but so useful.

time_t ref_time;
time_t cur_time;

int state = START;
char incomingByte;

int cur_animal = 0;
int cur_color = 0;

char *animal_list[] = {"ape","baboon","crow","dog","eagle","fox","gibbon","hare","ibis","jaguar",
                       "kestrel","lion","mink","newt","ostrich","parrot","quetzal","rabbit","snake",
                       "tiger","urchin","viper","whale","xerus","yak","zebra"};
char *color_list[] = {"amber","bronze","carmine","drab","emerald","ferrari","gold","hot pink","iris",
                      "jade","kelly green","lavender","mandarin","navy blue","olive","pastel",
                      "raspberry","salmon","taupe","umber","vermilion","white","xanadu","yellow",
                      "zinnwaldite brown"};

Thread animalThread = Thread();
Thread colorThread = Thread();
Thread printAllInfoThread = Thread();
ThreadController controller = ThreadController();

// this function is called by the animal thread
void update_animal() {
  if (cur_animal > 24) 
    cur_animal = 0;
  else
    cur_animal++;
}

// this function is called by the fruit thread
void update_color() {
  if (cur_color > 24)
    cur_color = 0;
  else 
    cur_color++;  
}

// this function is called by the print info thread
void print_all_info() {
  Serial.print(cur_time);
  Serial.print("\t");
  Serial.print(animal_list[cur_animal]);
  Serial.print("\t");
  Serial.println(color_list[cur_color]);
}

void setup() {
  Serial.begin(9600);

  animalThread.enabled = true;
  animalThread.setInterval(300);
  animalThread.onRun(update_animal);
  
  colorThread.enabled = true;
  colorThread.setInterval(2000);
  colorThread.onRun(update_color);

  printAllInfoThread.enabled = true;
  printAllInfoThread.setInterval(301);
  printAllInfoThread.onRun(print_all_info);
  
  controller.add(&animalThread);
  controller.add(&colorThread);
  controller.add(&printAllInfoThread);

  ref_time = now();
  state = RUNNING;
}

void loop() {
  cur_time = now() - ref_time;

  switch(state) {
    case RUNNING:
      controller.run();
      break;
    case RESET:
      cur_color = 0;
      cur_animal = 0;
      ref_time = now();
      state = RUNNING;
      Serial.println("------------------------------");
      break;
    default:
      Serial.println("Something's gone very wrong.");
      break;
  }

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'A')
      state = RESET;
    else 
      Serial.println("Press 'A' to RESET");
  }
}
