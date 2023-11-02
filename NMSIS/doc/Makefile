# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build
DOXYDIR       = $(BUILDDIR)/html/doxygen
# require NUCLEI_SDK_ROOT environment variable to be set to root of Nuclei SDK
HTTPSERVER    ?= $(NUCLEI_SDK_ROOT)/tools/scripts/nsdk_cli/httpserver.py

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile preview

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

all:
	make doxygen_core
	make doxygen_dsp
	make doxygen_nn
	make html

clean_xml:
	rm -rf $(DOXYDIR)/core/xml/
	rm -rf $(DOXYDIR)/dsp/xml/
	rm -rf $(DOXYDIR)/nn/xml/

doxygen_core:
	mkdir -p $(DOXYDIR)
	cd source/doxygen && doxygen nmsis_core.dxy

doxygen_dsp:
	mkdir -p $(DOXYDIR)
	cd source/doxygen && doxygen nmsis_dsp.dxy
	cp source/asserts/images/dsp/* $(DOXYDIR)/dsp/xml/

doxygen_nn:
	mkdir -p $(DOXYDIR)
	cd source/doxygen && doxygen nmsis_nn.dxy
	cp source/asserts/images/nn/* $(DOXYDIR)/nn/xml/

firefox:
	firefox build/html/index.html

firefox_pdf:
	firefox build/latex/nmsis.pdf

preview:
	$(HTTPSERVER) -d build/html
