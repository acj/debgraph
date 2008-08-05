DOCBOOK_HTML=docbook2html
DOCBOOK_PDF=docbook2pdf
DOCBOOK_MAN=docbook-to-man

all: html manpage pdf

clean:
	rm -rf html manpage pdf

html:
	$(DOCBOOK_HTML) debgraph.docbook -o html

manpage:
	$(DOCBOOK_MAN) debgraph.docbook -o man

pdf:
	$(DOCBOOK_PDF) debgraph.docbook -o pdf
