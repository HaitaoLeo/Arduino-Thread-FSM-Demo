// Thread Demo Sketch

// Uses the ArduinoThread library
// by Ivan Seidel
// https://github.com/ivanseidel/ArduinoThread

#include <ThreadController.h>
#include <Thread.h>
#include <TimeLib.h>
#include <Time.h>

time_t ref_time;
time_t cur_time;

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
  printAllInfoThread.setInterval(310);
  printAllInfoThread.onRun(print_all_info);
  
  controller.add(&animalThread);
  controller.add(&colorThread);
  controller.add(&printAllInfoThread);

  ref_time = now();
  Serial.println(ref_time);
}

void loop() {
  cur_time = now() - ref_time;
  controller.run();
}
