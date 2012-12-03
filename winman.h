#include <X11/Xlib.h>

typedef struct {
  Window window;
  int id;
} App;

void dbg(char*);
void reorganize();
App *getApp(Window);
