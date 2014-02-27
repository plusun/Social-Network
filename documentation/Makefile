document.pdf: document.tex
	pdflatex $^ && pdflatex $^
tidy:
	rm -rf *.dvi *.log *~ *.aux *.toc auto/
.PHONY: clean
clean: tidy
	rm -f document.pdf


