# GRACE - GRaphing, Advanced Computation and Exploration of data

Recommended reading:

```
COPYRIGHT               - legal stuff
LICENSE                 - license terms (GPL)
CHANGES                 - chronological list of changes
doc/FAQ.html            - frequently asked questions (in HTML format)
```

The installation procedure is covered in the User's Guide
(in the doc directory).

# My fork additions

Direct fork from [fxcoudert/xmgrace](https://github.com/fxcoudert/xmgrace).<br/>
[XmGrace](http://plasma-gate.weizmann.ac.il/Grace/) with a more appealing look by replacing symbols to Pentagon, Hexagon, and Octagon instead of triangle variants.
Adding routine to plot symbols plus, cross, and star as polygons instead of lines allowing filling with color.
Introducing slithly modified Default.agr with colors similar to [seaborn](https://seaborn.pydata.org/)/[matplotlib](https://matplotlib.org/), see figure below.

## ChangeLog

- change Default.agr for more pleasing appearance
  - line thickness
  - font Helvetica in most dialogs
  - suitable DIN A4 plotting screen
  - pleasing colors as in seaborn/matplotlib
    
- adding plotting routine to replace symbols by polygons
  - rename Triangle up -> Triangle
  - reamme Triangle left -> Pentagon
  - rename Triangle down -> Hexagon
  - rename Triangle right -> Octagon
  - replace drawing for plus, cross, and star
  - see line 1945-1999 in platone.c for triangle fan as in matplotlib

- default behavior: ps file export at start-up

## Install

````sh
    # configures the application - see also doc/UsersGuide.pdf
    ./configures
    # build the application
    make
    # run the application
    ./src/xmgrace
    # install the application with root
    sudo make install
````

## Screenshot
<img src="https://user-images.githubusercontent.com/16195016/115552842-07de8e80-a2ad-11eb-8e5a-a3fbd8dfd998.png" width="400" />

## Useful codies
````sh
    # use inkscape to convert ps to png
    inkscape --export-type="png" -D xmgrace_plot.ps
    # use ghostview to convert ps to pdf
    gs -dCompatibilityLevel=1.4 -sPAPERSIZE=a4 -dDELAYSAFER -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -dEmbedAllFonts=true -sOutputFile=xmgrace_plot.pdf -DPDFSETTINGS=/prepress -c "<</Orientation 3>> setpagedevice" -f xmgrace_plot.ps
````

## Useful links
[Unicode/LaTex font support](https://github.com/alejandrogallo/xmgrace): install of additional fonts and changes in Default.agr<br/>
[XmGrace .agr-parser](https://github.com/goerz/xmgrace_parser): python module for parsing .agr files<br/>
[JScatter xmgrace-binding](https://gitlab.com/biehl/jscatter/-/blob/master/src/jscatter/graceplot.py): use python3 to access xmgrace<br/>
[XmGrace patch by feranick](https://github.com/feranick/grace-extended): after build xmgrace looks amazing and is more intuitive to use<br/>
[XmGrace patch by vigmond](https://github.com/vigmond/xmgrace-patch): after build xmgrace looks even more amazing and is more intuitive to use (but sadly breaks the default agr.file in non critical way)<br/>

## Acknowledgement

Big thanks to the XmGrace community to keep it alive :)
