all: gen

gen:
	@echo "Generate rst document from doxygen doc"
	cd ../../doc && make doxygen_dsp doxygen_nn
	python3 doxy2rst.py

sync: source
	@echo "Sync generated rsts to real locations"
	rm -rf ../../doc/source/dsp/api/
	cp -r source/dsp/api ../../doc/source/dsp/
	rm -rf ../../doc/source/nn/api/
	cp -r source/nn/api ../../doc/source/nn/

clean:
	@echo "Clean generated source"
	rm -rf source
