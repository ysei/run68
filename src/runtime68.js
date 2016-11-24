/* global LibraryManager */
/* global Module */
/* global magic2 */
/* global mergeInto */
mergeInto(LibraryManager.library, {
    jsrt_magic2: function (cmd_adr) {
        if (magic2.auto(Module.HEAPU8, cmd_adr))
            return 5;
        return 0;
    }
});