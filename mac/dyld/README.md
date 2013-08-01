# dyld on Darwin

"inject-mine" is modified from [KOICHI TAMURA'S BLOG](http://koichitamura.blogspot.hk/2008/11/hooking-library-calls-on-mac.html).

## Object Lookup Tools

   * `nm`
   * `gobjdump`, the GNU `objdump`
   * `otool`, shipped with XCode. People say it's better.
   * `dwarftool`

## Linux Related Notes

### `LD_LIBRARY_PATH` and `LD_PRELOAD`

from `man ld.so`:

> `LD_LIBRARY_PATH`
>        A **colon-separated** list of directories in which to search for ELF libraries at execution-time.  Similar
>        to the PATH environment variable.
>
> `LD_PRELOAD`
>        A **whitespace-separated** list of additional, user-specified, ELF shared libraries to be loaded before all
>        others.   This  can be used to selectively override functions in other shared libraries.  For set-user-
>        ID/set-group-ID ELF binaries, only libraries in the standard search directories that are also set-user-
>        ID will be loaded.

The short description suggests:

   * `LD_LIBRARY_PATH` provides **directories**.
   * `LD_PRELOAD` provides **pathes** (filenames).

[This thread](http://stackoverflow.com/questions/14715175/what-is-the-difference-between-ld-preload-path-and-ld-library-path) 
suggests that:

   * `LD_LIBRARY_PATH` provides search dir.
   * `LD_PRELOAD` will preload the functions either the program requests or not.

[This thread](http://www.linuxquestions.org/questions/linux-general-1/differences-between-ld_library_path-and-ld_preload-235321/)
suggests that:

   * `LD_LIBRARY_PATH` is used to override a whole library.
   * `LD_PRELOAD` is used to override a specific function.
   (however, libs like `dsocks` use it to rewrite nearly a whole SOCKET library).

## References

   * The discussion of how to get `objdump`.
   <http://superuser.com/questions/206547/how-can-i-install-objdump-on-mac-os-x>.
   It's available via `brew install binutils`.
   Note that the name is prefixed with a "g", i.e. `gobjdump`.
   * General knowledge of dynamic linker.
   <http://en.wikipedia.org/wiki/Dynamic_linker>.
   * Simple code injection using `DYLD_INSERT_LIBRARIES`.
   <http://blog.timac.org/?p=761>.
   Offers more alternatives of code injection on Max OS X than `DYLD_INSERT_LIBRARIES`.
   Inject the calculator.
   Use of Obj-C.
   * Overriding library functions in Mac OS X, the easy way: `DYLD_INSERT_LIBRARIES`.
   <http://tlrobinson.net/blog/2007/12/overriding-library-functions-in-mac-os-x-the-easy-way-dyld_insert_libraries/>.
   * Hooking library calls on Mac using `DYLD_INSERT_LIBRARIES`.
   <http://koichitamura.blogspot.hk/2008/11/hooking-library-calls-on-mac.html>
   * man page of `dyld`.
   <http://developer.apple.com/library/mac/#documentation/Darwin/Reference/Manpages/man1/dyld.1.html>
   * Linux library HOWTO -- Shared Libraries,
   <http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html>

