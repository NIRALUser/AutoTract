# AutoTract

AutoTract is an automatic tractography tool featuring advanced processing tools (using FiberPostProcess) to clean tracts after the initial tractography (done with the Slicer module TractographyLabelMapSeeding).

## Full Documentation

Find the tool on [NITRC](https://www.nitrc.org/projects/autotract/) as well as the [tutorial](https://www.nitrc.org/docman/?group_id=948).

## Data Required

- a DTI image that you want to tract
- a reference DTI atlas + a folder containing the tracts of this atlas (this will serve as label maps for the tractography)
- (optional) a WM mask
- (optional) a CSF mask

## Dependencies

### Packages

- ITK (v4.7 and above) [download page](http://www.itk.org/ITK/resources/software.html) 
- QMAKE (QT4) [download page](https://www.qt.io/download/)
- QTGUI / QTtToCppXml [source code/release on github](https://github.com/fbudin69500/QTGUI)
- SlicerExecutionModel [source code/release on github](https://github.com/Slicer/SlicerExecutionModel)
- VTK (v6.1.0 and above) [download page](http://www.vtk.org/download/)

### Softwares

- DTI-Reg (requires ANTS, ITKTransformTools, dtiprocess, ResampleDTIlogEuclidean) [NITRC page](https://www.nitrc.org/projects/dtireg/)
- ANTS  [download page](http://stnava.github.io/ANTs/)
- ITKTransformTools [source code/release on github](https://github.com/NIRALUser/ITKTransformTools)
- dtiprocess (part of DTIProcessToolkit) [source code on github](https://github.com/NIRALUser/DTIProcessToolkit)
- Resample DTIlogEuclidean [source code/release on github](https://github.com/NIRALUser/ResampleDTIlogEuclidean)
- Slicer [download page](http://slicer.kitware.com/midas3/slicerpackages/view)
- fiberprocess (part of DTIProcessToolkit) [source code on github](https://github.com/NIRALUser/DTIProcessToolkit)
- ImageMath (part of niral_utilities) [source code on github](https://github.com/NIRALUser/niral_utilities)
- TractographyLabelMapSeeding (Slicer module) [download page](http://slicer.kitware.com/midas3/slicerpackages/view)
- FiberPostProcess (part of DTIFiberTractStatistics) [source code on github](https://github.com/NIRALUser/DTIFiberTractStatistics)
- polydatatransform (part of niral_utilities) [source code on github](https://github.com/NIRALUser/niral_utilities)
- unu (part of teem) [download page](http://teem.sourceforge.net/download/)
- MaurerDistanceTransform (part of niral_utilities) [source code on github](https://github.com/NIRALUser/niral_utilities)
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

## License

See License.txt

