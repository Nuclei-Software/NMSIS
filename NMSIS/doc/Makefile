# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

all:
	make doxygen_core
	make doxygen_dsp
	make doxygen_nn
	make html

doxygen_core:
	mkdir -p build/doxygen
	cd source/doxygen && doxygen nmsis_core.dxy

doxygen_dsp:
	mkdir -p build/doxygen
	cd source/doxygen && doxygen nmsis_dsp.dxy
	cp source/asserts/images/dsp/* build/doxygen/dsp/xml/

doxygen_nn:
	mkdir -p build/doxygen
	cd source/doxygen && doxygen nmsis_nn.dxy
	cp source/asserts/images/nn/* build/doxygen/nn/xml/

doxygen:
	doxygen nmsis.doxy

doxygen_pdf: doxygen
	make -C build/doxygen/latex pdf

firefox:
	firefox build/html/index.html

firefox_pdf:
	firefox build/latex/nmsis.pdf
