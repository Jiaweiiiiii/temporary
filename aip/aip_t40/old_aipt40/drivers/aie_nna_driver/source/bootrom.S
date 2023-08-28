
/* bootrom used for ITC */
/* just jump to ITC kernel entry. */
/* you can modify it and add more function code here. */

.global _bootrom_entry
_bootrom_entry:
    .set   push
    .set   noreorder
    .set   noat
    mfc0    $26,  $16, 0
    ins     $26,  $0,  0, 3
    ori     $26,  $26, 0x3 /* k0 cacheable */
    mtc0    $26,  $16, 0   /* page is cacheable. */
    lui     $1, 0x8001
    jr.hb   $1
    nop
    .set   pop

