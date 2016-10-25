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

int iocs_call() {
  UChar no = rd[0] & 0xff;
  switch (no) {
    case 0x81:  // B_SUPER
      if (func_trace_f)
        printf( "$%06x IOCS(%02X): B_SUPER\n", pc - 2, no);
      super();
      break;
    default:
      if (func_trace_f)
        printf( "$%06x IOCS(%02X): NOT IMPL.\n", pc - 2, no);
      break;
  }
  return 0;
}
