# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
# sys.path.insert(0, os.path.abspath('.'))

# -- Project information -----------------------------------------------------

project = 'NMSIS'
copyright = '2019-Present, Nuclei'
author = 'Nuclei'

# The short X.Y version
version = '1.3.1'

# The full version, including alpha/beta/rc tags
release = '1.3.1'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx_rtd_theme',
    'breathe',
    'sphinx.ext.githubpages',
    'sphinx.ext.mathjax',
    'recommonmark'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------
# Show build timestamp
html_last_updated_fmt = ""

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

html_logo = 'asserts/images/nmsis_logo.png'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

html_theme_options = {
    'logo_only': True,
    'navigation_depth': 6
}

# -- Options for Breathe Project ---------------------------------------------
breathe_projects = {
    "nmsis_core": "../build/html/doxygen/core/xml",
    "nmsis_dsp": "../build/html/doxygen/dsp/xml",
    "nmsis_nn": "../build/html/doxygen/nn/xml"
}
breathe_default_project = "nmsis_core"
breathe_show_define_initializer = True

# -- Options for Latex output -------------------------------------------------
latex_logo = 'asserts/images/nmsis_logo.png'
latex_show_pagerefs = True
latex_toplevel_sectioning = 'chapter'
latex_show_urls = 'footnote'

rst_epilog = """
.. |nuclei_core| replace:: Nuclei N/NX Class Processors
.. |nuclei_ncore| replace:: Nuclei N Class Processors
.. |nuclei_nxcore| replace:: Nuclei NX Class Processors
.. |nmsis_support_cores| replace:: N200, N300, N600, NX600
.. |nmsis_core_defines| replace:: **NUCLEI_N200**, **NUCLEI_N300**, **NUCLEI_N600** or **NUCLEI_NX600**
"""
# -- Extension configuration -------------------------------------------------

def setup(app):
    app.add_css_file("css/custom.css")
