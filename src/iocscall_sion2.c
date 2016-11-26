#undef MAIN

#include "run68.h"

static void super() {
  if (!ra[1]) {
    // to SUPER
    rd[0] = ra[7];
    SR_S_ON();
  } else {
    // to USER
    ra[7] = ra[1];
    rd[0] = 0;
    SR_S_OFF();
  }
}

#if defined(EMSCRIPTEN)
extern int jsrt_iocs_bitsns(int group);
extern int jsrt_iocs_contrast(UChar c);
extern int jsrt_iocs_joyget(int id);
extern void jsrt_iocs_sp_on();
extern void jsrt_iocs_sp_off();
extern void jsrt_iocs_sp_regst(
    ULong id, ULong x, ULong y, ULong code, ULong prio);
// FIXME: rename to jsrt_iocs_gpalet.
extern void jsrt_io_graphic_palette(UShort index, UShort color);
#endif

int iocs_call() {
  UChar no = rd[0] & 0xff;
  switch (no) {
    case 0x04:  // BITSNS
#if defined(EMSCRIPTEN)
      rd[0] = jsrt_iocs_bitsns(rd[1]);
#else
      rd[0] = 0;
#endif
      break;
#if defined(EMSCRIPTEN)
    case 0x11:  // CONTRAST
      if (rd[1] < 0)
        printf( "$%06x IOCS(CONTRAST): read is not impl.\n", pc - 2);
      jsrt_iocs_contrast(rd[1]);
      break;
#endif
    case 0x3b:  // JOYGET
#if defined(EMSCRIPTEN)
      rd[0] = jsrt_iocs_joyget(rd[1]);
#else
      rd[0] = 0xff;
#endif
      break;
    case 0x81:  // B_SUPER
      super();
      break;
#if defined(EMSCRIPTEN)
    case 0x94:  // GPALET
      jsrt_io_graphic_palette(rd[1], rd[2]);
      break;
    case 0xc1:  // SP_ON
      jsrt_iocs_sp_on();
      break;
    case 0xc2:  // SP_OFF
      jsrt_iocs_sp_off();
      break;
    case 0xc6:  // SP_REGST
      jsrt_iocs_sp_regst(rd[1], rd[2], rd[3], rd[4], rd[5]);
      break;
    case 0xcc:  // BGTEXTCL
      // TODO
      break;
    case 0xcd:  // BGTEXTST
      // TODO
      break;
#endif
    default:
      printf( "$%06x IOCS(%02X): NOT IMPL.\n", pc - 2, no);
      break;
  }
  return 0;
}
