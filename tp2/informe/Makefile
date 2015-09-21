PARAMS="-halt-on-error -file-line-error -output-format pdf"

all: informe bibliography 

informe: informe.tex
	pdflatex $(PARAMS) informe.tex

%.bcf: %.tex
	pdflatex $(PARAMS) $<

%.pdf: %.tex
	pdflatex $(PARAMS) $<

bibliography: informe.bcf
	biber informe.bcf

clean:
	rm -f informe.pdf *.aux *.log *.bcf *.out *.run.xml *.toc *.bbl *.blg
