# AutoTract

AutoTract is an automatic tractography tool featuring advanced processing tools (using FiberPostProcess) to clean fiber tracts after the initial tractography (done with the Slicer module TractographyLabelMapSeeding).

## Full Documentation

Find the software on [NITRC](https://www.nitrc.org/projects/autotract/) as well as its [tutorial](https://www.nitrc.org/docman/view.php/948/1969/Tutorial.pdf).

## Required Data

- a DTI image on which you want to perform fiber tractography
- a reference DTI atlas + a folder containing the tracts of this atlas (this will serve as label maps for the tractography)
- (optional) a WM mask
- (optional) a CSF mask

## Dependencies

### Packages

- ITK (v4.7 and above) [download page](http://www.itk.org/ITK/resources/software.html) 
- Qt4 [download page](https://www.qt.io/download/)
- QTGUI / QTtToCppXml [source code](https://github.com/fbudin69500/QTGUI)
- SlicerExecutionModel [source code](https://github.com/Slicer/SlicerExecutionModel)
- VTK (v6.1.0 and above) [download page](http://www.vtk.org/download/)

### Software

- DTI-Reg (requires ANTS, ITKTransformTools, dtiprocess, ResampleDTIlogEuclidean) [NITRC page](https://www.nitrc.org/projects/dtireg/)
- ANTS  [source code](http://stnava.github.io/ANTs/)
- ITKTransformTools [source code](https://github.com/NIRALUser/ITKTransformTools)
- DTIProcessToolkit (for dtiprocess and fiberprocess) [NITRC page](https://www.nitrc.org/projects/dtiprocess/)
- niral_utilities (for ImageMath, PolyDataTransform, MaurerDistanceTransform) [NITRC page](https://www.nitrc.org/projects/niral_utilities/)
- ResampleDTIlogEuclidean [source code](https://github.com/NIRALUser/ResampleDTIlogEuclidean)
- Slicer (for TractographyLabelMapSeeding) [download page](http://download.slicer.org/)
- DTIFiberTractStatistics (for FiberPostProcess) [NITRC page](https://www.nitrc.org/projects/dti_tract_stat/)
- unu (part of teem) [download page](http://teem.sourceforge.net/download/)
- python (v2.7.5 and above) [download page](https://www.python.org/downloads/)

## Command Line

```
AutoTract --noGui --parameters [parameters_config_file.xml] --executables [executables_config_file.xml]
```

Note: An easy way to generate the basic XML configuration files is to launch AutoTract with the GUI and click File->Save Parameter/Executable Configuration, it will generate the XML file and you can modify the values in that file afterwards.

## Future Improvements

- Post Processing comparing the shape of the reference tract (by generating an "average" tract? curvature? bounding box?) to further refine the processing
- Tract specific parameters
- Log window with the option of showing details on each tract process
- CMake using Superbuild
- Additional unit and integration tests
- Detection of the end of the pipeline is not managed by the UI (run/stop pushButtons do not change once the pipeline ends) 

## License

See LICENSE

