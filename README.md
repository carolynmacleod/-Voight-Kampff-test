# -Voight-Kampff-test

ESG is an equal opportunity employer. Both replicants and humans can complete this test and be considered for employment.  Note that "retirement" on failure is *not* practiced at ESG.

Disclaimer: Above references to *Blade Runner* are included because we are sci-fi nerds. Have fun working on this homework -- we're looking forward to seeing what you can do.


**The Challenge**

Implement a stand alone CLI application that takes the data\event_data.csv file input containing:
geophysical coordinates (east, north, depth) that represent located seismic events and their associated moment magnitude (Mw)

In C++ and using VTK (Visualization ToolKit):
1) Implement a data visualization pipeline of the events as solid spheres colored by moment magnitude.

2) Represent the data bounds of the event locations with a simple wireframe cube.

3) Provide a vertical scale that shows the relation of color to moment magnitude.

4) Fit a 3D wireframe ellipsoid to the event locations using PCA analysis (do not weight the points) using a sensible scaling strategy.

5) Ensure the initial focal point of the scene is centered on the event locations and that the scene can be interactively rotated/zoomed/panned by mouse.


**Suggested Build Workflow**

1. create build directory for vtk

2. copy cmake\preload.cmake to vtk build directory

3. in vtk build dir:
   - cmake -C preload.cmake path_to_vtk_source
   - cmake .
   - cmake --build . --config Debug

4. create build directory for vtk_challenge and in that dir
   - cmake -S path_to_vtk_challenge -B . -G "Visual Studio 17 2022" -A x64 -DVTK_DIR=path_to_vtk_build
   - cmake --build . --config Debug
   - cd Debug
   - vtk_challenge.exe path_to_event_data_file
