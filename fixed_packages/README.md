# Fixed Packages #

Turns out the `libnewlib-arm-none-eabi` and `libnewlib-dev` packages are broken
in Ubuntu 18.04. The packages you get from `apt-get` were apparently compiled
with the wrong compiler and aren't going to work. These packages are from
Ubuntu 18.10, and have been fixed. The `./bootstrap.sh` script should detect
the bad packages and install these good ones.

**See also:**

  * https://github.com/qmk/qmk_firmware/issues/4209
  * https://bugs.launchpad.net/ubuntu/+source/newlib/+bug/1767223
