# succobt
This is **succobt**, a set of tools for early manipulation and online analysis of the data collected at INSULAb-like beamtests &mdash; see, e.g., [this presentation](https://indico.cern.ch/event/731649/contributions/3237202/) and [this other project](https://github.com/mattiasoldani/anaKrys). In particular

- `asciinroot_to_root_pp/` takes the data output of the DAQ system (both waveform ROOT files and ASCII files with tracking data and digitizer fast analysis) and merges all the single-event information in dedicated level-2 ROOT files;
- `ascii_stripper_merger/` takes the DAQ output ASCII files (typically one file per spill) and merges all the files of a single run into a single ASCII file;
- `wf_analysis_preliminary/` takes the level-2 ROOT files and performs preliminary waveform analysis of a digitizer channel.

### Notes on `asciinroot_to_root_pp/`

Two versions of the tool are available:

- "_NOASCII" only merges single-digitizer waveform ROOT files, without any information from tracking or fast analysis;
- "_ASCIINROOT" takes the single-digitizer waveform information for the events available in the ASCII files and combines it with the information in the corresponding ASCII entries.

Either way, the core of the event reconstruction is in the `asciinroot_to_root_pp_[VERSION].cc` file. It can be run automatically as the data are collected and the DAQ output is produced (`auto_update_root_files_[VERSION].sh`) or manually on a selected set of data runs (`manual_run_select_root_files_[VERSION].sh`). Further details can be found in the comments to the source code.

### Notes on `wf_analysis_preliminary/`

The analysis runs in a Jupyter notebook, `wf_analysis_preliminary.ipynb`. If explicitly required in the notebook, all the generated output (plots and text files) go into the `output/` folder.

The analysis dependencies are:

[![python](https://img.shields.io/badge/python-3-blue.svg)](https://www.python.org/) [![awkward](https://img.shields.io/badge/awkward-grey.svg)](https://awkward-array.org/doc/main/) [![jupyterlab](https://img.shields.io/badge/jupyterlab-grey.svg)](https://jupyterlab.readthedocs.io/en/stable/) [![matplotlib](https://img.shields.io/badge/matplotlib-grey.svg)](https://matplotlib.org/) [![numpy](https://img.shields.io/badge/numpy-grey.svg)](https://numpy.org/) [![pip](https://img.shields.io/badge/pip-grey.svg)](https://pip.pypa.io/en/stable/) [![scipy](https://img.shields.io/badge/scipy-grey.svg)](https://www.scipy.org/)  [![succolib](https://img.shields.io/badge/succolib-grey.svg)](https://github.com/mattiasoldani/succolib) [![uproot](https://img.shields.io/badge/uproot-grey.svg)](https://github.com/scikit-hep/uproot) 
