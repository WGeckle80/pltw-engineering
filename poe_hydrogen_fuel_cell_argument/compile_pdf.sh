#!/bin/bash

# Ubuntu packages installed (may be bloated):
#   *texlive-latex-extra
#   *texlive-bibtex-extra
#   *biber

# Independent packages installed:
#   *apa7.cls

pdflatex hydrogen_fuel_cell_argument.tex
biber hydrogen_fuel_cell_argument
pdflatex hydrogen_fuel_cell_argument.tex
pdflatex hydrogen_fuel_cell_argument.tex

rm hydrogen_fuel_cell_argument.aux
rm hydrogen_fuel_cell_argument.log
rm hydrogen_fuel_cell_argument.bbl
rm hydrogen_fuel_cell_argument.blg
rm hydrogen_fuel_cell_argument.bcf
rm hydrogen_fuel_cell_argument.out
rm hydrogen_fuel_cell_argument.run.xml
