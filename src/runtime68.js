/* global LibraryManager */
/* global Module */
/* global iocs_bitsns */
/* global magic2 */
/* global mergeInto */
// TODO:
// - Emulate characters.
// - Emulate stars.
mergeInto(LibraryManager.library, {
  jsrt_magic2: function(cmd_adr) {
    if (magic2.auto(Module.HEAPU8, cmd_adr))
      return 5;
    return 0;
  },
  jsrt_iocs_bitsns: function(group) {
    return iocs_bitsns(group);
  },
  jsrt_io_graphic_palette: function(index, color) {
    magic2.palette(index, color);
  }
});