# Test checking that the help can be printed even if no input file is given.
add_test(NAME AutoTractHelp COMMAND $<TARGET_FILE:AutoTract>
    --help
  )

# Test checking that the version can be printed even if no input file is given.
add_test(NAME AutoTractVersion COMMAND $<TARGET_FILE:AutoTract>
    --version
  )

set_tests_properties(AutoTractVersion PROPERTIES FAIL_REGULAR_EXPRESSION "Unspecified" )

# Unit tests checking that every executable is on the system

# Check DTI-Reg
# Check ANTS
# Check ITKTransformTools
# Check dtiprocess
# Check fiberprocess


# Integration tests checking that each step works by comparing the output

## Full integration step: run the program, check that it returns 0

## Registration step: check that the step is run properly (generation of a file "displacementField.nrrd" in [output_folder]/1.Registration/)

## Registration step: given a parameter file that contains a displacement field (or if the file "displacementField.nrrd" exists in [output_folder]/1.Registration/,
## check that the registration is skipped (read Output.log and check that the string 
## "=== Registration ===
## -> Skipped" exists.)

## MaskCreation step: check that the output masks (WMmask.nrrd and MDmask.nrrd) are identical to a reference data

## MaskCreation step: check that if there is a WM mask and a MD mask given in the input tab, the mask creation step is skipped (or if the WM mask/MD mask exists in [output_folder]/2.MaskCreation/)

## Processing step: check/compare the final output fiber tract, the intermediate output fiber tracts
