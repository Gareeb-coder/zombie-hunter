#ifndef DISPLAY_H
#define DISPLAY_H

#define CONSOLE_OUT wcout

class Display {
  unsigned int rows;
  unsigned int cols;
public:
  Display();
  virtual ~Display();
  unsigned int getRows(void);
  unsigned int getCols(void);
  void clear();
  void print(const char * string,...);
  void getanykey();
  void draw();
};

extern "C" {
    void sleepy(unsigned int milliSeconds);
}

#endif
