#undef MAIN

#include <stdint.h>
#include <stdlib.h>
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

#if defined(EMSCRIPTEN)
extern int jsrt_magic2(char* cmd_adr);
#endif

int magic2_call() {
#if defined(EMSCRIPTEN)
  return jsrt_magic2(&prog_ptr[ra[0]]);
#else
  long cmd_adr = ra[0];
  if (func_trace_f)
    printf("$%06lx MAGIC2\n", pc - 2);
  for (short cmd = 0; (cmd = mem_get(cmd_adr, S_WORD)) != DONE; cmd_adr += 2) {
    switch (cmd) {
      case SET_WINDOW: {
        UShort x1 = mem_get(cmd_adr + 2, S_WORD);
        UShort y1 = mem_get(cmd_adr + 4, S_WORD);
        UShort x2 = mem_get(cmd_adr + 6, S_WORD);
        UShort y2 = mem_get(cmd_adr + 8, S_WORD);
        cmd_adr += 8;  // x1, y1, x2, y2
        printf("    { \"command\": \"set window\",\n");
        printf("      \"data\": "
            "{ \"x1\": %d, \"y1\": %d, \"x2\": %d, \"y2\": %d } },\n",
            x1, y1, x2, y2);
        break; }
      case SET_MODE: {
        UShort mode = mem_get(cmd_adr + 2, S_WORD);
        cmd_adr += 2;  // mode
        printf("    { \"command\": \"set mode\",\n");
        printf("      \"data\": { \"mode\": %d } },\n", mode);
        break; }
      case CLS:
        printf("    { \"command\": \"cls\" },\n");
        break;
      case SET_3D_PARAM: {
        UShort num = mem_get(cmd_adr + 2, S_WORD);
        UShort data = mem_get(cmd_adr + 4, S_WORD);
        cmd_adr += 4;  // param, data
        printf("    { \"command\": \"set 3d parameter\",\n");
        printf("      \"data\": { \"num\": %d, \"data\": %d } },\n", num, data);
        break; }
      case SET_3D_DATA: {
        UShort vertices = mem_get(cmd_adr + 2, S_WORD);
        cmd_adr += 2;
        printf("    { \"command\": \"set 3d data\",\n");
        printf("      \"data\": {\n");
        printf("        \"pct\": %d,\n", vertices);
        printf("        \"vertices\": [\n");
        for (int i = 1; i <= vertices; ++i) {
          int16_t x = mem_get(cmd_adr + 2, S_WORD);
          int16_t y = mem_get(cmd_adr + 4, S_WORD);
          int16_t z = mem_get(cmd_adr + 6, S_WORD);
          printf("          { \"x\": %d, \"y\": %d, \"z\": %d }%s\n", x, y, z,
              i == vertices ? " ]," : ",");
          cmd_adr += 6;
        }
        UShort lines = mem_get(cmd_adr + 2, S_WORD);
        cmd_adr += 2;
        printf("        \"lct\": %d,\n", lines);
        if (ext_color) {
          UShort color = mem_get(cmd_adr + 2, S_WORD);
          printf("        \"color\": %d,\n", color);
          cmd_adr += 2;
        }
        printf("        \"lines\": [\n");
        for (int i = 1; i <= lines; ++i) {
          UShort ls = mem_get(cmd_adr + 2, S_WORD);
          UShort le = mem_get(cmd_adr + 4, S_WORD);
          printf("          { \"ls\": %d, \"le\": %d }%s,\n", ls, le,
              i == lines ? " ] } }" : "");
          cmd_adr += 4;
        }
        break; }
      case TRANSLATE:
        printf("    { \"command\": \"translate 3d->2d\" },\n");
        break;
      case DISPLAY:
        printf("    { \"command\": \"display 2d\" },\n");
        return 5;
      case COLOR: {
        UShort color = mem_get(cmd_adr + 2, S_WORD);
        cmd_adr += 2;  // color
        printf("    { \"command\": \"color\",\n");
        printf("      \"data\": { \"color\": %d } },\n", color);
        break; }
      case CRT: {
        UShort mode = mem_get(cmd_adr + 2, S_WORD);
        if (mode & 0x100)
          ext_color = TRUE;
        cmd_adr += 2;  // mode
        printf("    { \"command\": \"crt\",\n");
        printf("      \"data\": { \"mode\": %d } },\n", mode);
        break; }
      case INIT:
        puts("{");
        puts("  \"type\": \"magic2\",");
        puts("  \"sequence\": [");
        break;
      case APAGE: {
        UShort page = mem_get(cmd_adr + 2, S_WORD);
        cmd_adr += 2;  // page
        printf("    { \"command\": \"apage\",\n");
        printf("      \"data\": { \"page\": %d } },\n", page);
        break; }
      case DEPTH: {
        UShort minz = mem_get(cmd_adr + 2, S_WORD);
        UShort maxz = mem_get(cmd_adr + 4, S_WORD);
        cmd_adr += 4;  // minz, maxz
        printf("    { \"command\": \"depth\",\n");
        printf("      \"data\": { \"minz\": %d, \"maxz\": %d } },\n",
            minz, maxz);
        break; }
      default:
        printf("UNKNOWN COMMAND: $%04x\n", cmd);
        abort();
    }
  }
  printf("    { \"command\": \"done\" },\n");
  return 0;
#endif
}

