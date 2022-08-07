Disk: I.C.E. Felix HC
====================

The HC (an abbreviation for 'home computer') was a series of ZX Spectrum   
clone personal computers produced by Romanian factory I.C.E. Felix.   

Several models were developed (HC-85, HC-90, HC-91, HC-91+, HC-2000) but only   
the last model (HC-2000) was equiped with an onboard floppy interface.    
An independent interface was produced called IF1 (based on sinclair   interface 1)   
with an integrated floppy disk drive, joystick and a network interface.   
The floppy drive controller used was a NEC uPD765/I8272 in MFM double density mode.   
More info about these computers can be [found here.](https://sites.google.com/site/georgechirtoaca/pictures?authuser=0)    


This used a variation of double-sided IBM scheme format with 16 sectors/256    
bytes for the BASIC disk and 9 sectors/512 bytes for the CP/M disk.   
Either 5.25" or 3.5" drives were used depending on what was available at the time,   
with the same format on both. The first models were equiped with a 5.25"/40 tracks
drive then a 3.5"/80 tracks drive was used.    
So we have 320Kb/640Kb disks for the BASIC format and 360Kb/720Kb disks for the CP/M format.   

Reading disks
-------------

Just do:

```
fluxengine read felix320 -o felix320.img
fluxengine read felix640 -o felix640.img
```
For BASIC disks (felix320 profile is for 5.25" disks)   

Or:   
```
fluxengine read felix360 -o felix360.img
fluxengine read felix720 -o felix720.img
```
For CP/M disks  (felix360 profile is for 5.25" disks)


Writing disks
-------------

Just do:

```
fluxengine write felix320 -o felix320.img
fluxengine write felix640 -i felix640.img
```
For BASIC disks (felix320 profile is for 5.25" disks)   

Or:   
```
fluxengine write felix360 -i felix320.img
fluxengine write felix720 -i felix720.img
```
For CP/M disks  (felix360 profile is for 5.25" disks)   

