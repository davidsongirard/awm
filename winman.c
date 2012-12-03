#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "winman.h"

static Display *disp;
static int screen;
static Window root;

static int screen_width;
static int screen_height;

static App *apps[100];
static int app_count;

int main(void) {

  if(!(disp = XOpenDisplay(0x0)))  {
    return 1;
  }

  root = DefaultRootWindow(disp);
  screen = DefaultScreen(disp);
  screen_width = DisplayWidth(disp, screen);
  screen_height = DisplayHeight(disp, screen);
  
  app_count = 0;

  XSetWindowAttributes windowAttr;
  windowAttr.event_mask = ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask;
  XChangeWindowAttributes(disp, root, CWEventMask, &windowAttr);
  XSelectInput(disp, root, windowAttr.event_mask);

  /* Primary event loop */
  while (1) {
    XEvent ev;
    XNextEvent(disp, &ev);

    /* Event for a new app being added */
    if (ev.type == MapRequest) {
      
      XMapRequestEvent *req = &ev.xmaprequest;
      XMapWindow(disp, req->window);
      
      App *new_app; 
      new_app = calloc(1, sizeof(App));
      new_app->window = req->window;
      new_app->id = app_count;

      apps[app_count] = new_app;
      app_count++;
      reorganize();

    /* Event for an app being removed */
    } else if (ev.type == UnmapNotify) {
      XUnmapEvent *req = &ev.xunmap;
      App *remove_me = getApp(req->window);
      remove_me->id = -1;
    }
  }
}

App *getApp(Window window) {
  int i;
  for (i = 0; i < app_count; i++) {
    if (apps[i]->id != -1 && window == apps[i]->window) {
      return apps[i];
    }
  }
  return NULL; 
}

void reorganize() {
  int i, num_windows = 0;
  for (i = 0; i < app_count; i++) {
    if (apps[i]->id != -1) {
      int x = num_windows * screen_width / 2; 
      XMoveResizeWindow(disp, apps[i]->window, x, 0, screen_width / 2, screen_height);
      num_windows++;
    }
  }
}

void dbg(char* message) {
  int fd = open("output", O_WRONLY | O_CREAT | O_APPEND, 0666);
  write(fd, message, strlen(message));
  close(fd);
  return;
}
