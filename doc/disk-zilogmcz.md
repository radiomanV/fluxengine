zilogmcz
====
## 320kB 8" 77-track SSSD hard-sectored
<!-- This file is automatically generated. Do not edit. -->

The Zilog MCZ is an extremely early Z80 development system, produced by
Zilog, which came out in 1976. It used twin 8-inch hard sectored floppy
drives; here's one at the <a
href="http://www.computinghistory.org.uk/det/12157/Zilog-Z-80-Microcomputer-System/">Centre
for Computing History</a>:

<div style="text-align: center">
<a href="http://www.computinghistory.org.uk/det/12157/Zilog-Z-80-Microcomputer-System/">
<img src="zilogmcz.jpg" style="max-width: 60%" alt="A Zilog MCZ at the Centre For Computing History"></a>
</div>

The MCZ ran Zilog's own operating system, Z80-RIO, and used 77 track
single-sided disks, with 32 sectors (each marked by an index hole), with 132
bytes per sector --- 128 bytes of user payload plus two two-byte metadata
words used to construct linked lists of sectors for storing files. These
stored 320kB each.

FluxEngine has experimental read support for these disks, based on a single
Catweasel flux file I've been able to obtain, which only contained 70 tracks.
I haven't been able to try this for real. If anyone has any of these disks,
an 8-inch drive, a FluxEngine and the appropriate adapter, please [get in
touch](https://github.com/davidgiven/fluxengine/issues/new)...

## Options

(no options)

## Examples

To read:

  - `fluxengine read zilogmcz -s drive:0 -o zilogmcz.img`

## References

  * [About the Zilog MCZ](http://www.retrotechnology.com/restore/zilog.html),
    containing lots of useful links

  * [The hardware user's manual](https://amaus.org/static/S100/zilog/ZDS/Zilog%20ZDS%201-25%20Hardware%20Users%20Manual.pdf)

