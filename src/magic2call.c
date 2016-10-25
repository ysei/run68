#undef MAIN

#include "run68.h"

enum {
  SET_WINDOW   = 0x0006,
  SET_MODE     = 0x0007,
  CLS          = 0x0009,
  SET_3D_PARAM = 0x000b,
  SET_3D_DATA  = 0x000c,
  TRANSLATE    = 0x000d,
  DISPLAY      = 0x000e,
  DONE         = 0x000f,
  COLOR        = 0x0010,
  CRT          = 0x0011,
  INIT         = 0x0012,
  APAGE        = 0x0014,
  DEPTH        = 0x0015,
};

int ext_color = FALSE;

int magic2_call() {
  long cmd_adr = ra[0];
  if (func_trace_f)
    printf("$%06x MAGIC2\n", pc - 2);
  for (short cmd = 0; (cmd = mem_get(cmd_adr, S_WORD)) != DONE; cmd_adr += 2) {
    switch (cmd) {
      case SET_WINDOW:
        puts(" -- SET WINDOW");
        cmd_adr += 8;  // x1, y1, x2, y2
        break;
      case SET_MODE:
        puts(" -- SET MODE");
        cmd_adr += 2;  // mode
        break;
      case CLS:
        puts(" -- CLS");
        break;
      case SET_3D_PARAM:
        puts(" -- SET 3D PARAM");
        cmd_adr += 4;  // param, data
        break;
      case SET_3D_DATA:
        puts(" -- SET 3D DATA");
        {
          UShort vertices = mem_get(cmd_adr + 2, S_WORD);
          cmd_adr += 2 + vertices * 6;
          UShort lines = mem_get(cmd_adr + 2, S_WORD);
          if (ext_color)
            cmd_adr += 2;
          cmd_adr += 2 + lines * 4;
        }
        break;
      case TRANSLATE:
        puts(" -- TRANSLATE");
        break;
      case DISPLAY:
        puts(" -- DISPLAY");
        break;
      case COLOR:
        puts(" -- COLOR");
        cmd_adr += 2;  // color
        break;
      case CRT:
        puts(" -- CRT");
        UShort mode = mem_get(cmd_adr + 2, S_WORD);
        if (mode & 0x100)
          ext_color = TRUE;
        cmd_adr += 2;  // mode
        break;
      case INIT:
        puts(" -- INIT");
        break;
      case APAGE:
        puts(" -- APAGE");
        cmd_adr += 2;  // page
        break;
      case DEPTH:
        puts(" -- DEPTH");
        cmd_adr += 4;  // near, far
        break;
      default:
        printf("  UNKNOWN COMMAND: $%04x\n", cmd);
        abort();
    }
  }
  return 0;
}

