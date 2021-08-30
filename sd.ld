SPI (xr)           : ORIGIN = 0x90000000, LENGTH = 8M

 /DISCARD/ :
  {
    *(.ARM.exidx*)
    *(.ARM.extab*)
  }
.text2 :
  {
    . = ALIGN(4);
    libs/librustzcash.a(.text)
    libs/libsnark.a(.text)
    libs/libsodium.a(.text)
    libs/librustzcash.a(.text*)
    libs/libsnark.a(.text*)
    libs/libsodium.a(.text*)
    . = ALIGN(4);
  } >SPI
