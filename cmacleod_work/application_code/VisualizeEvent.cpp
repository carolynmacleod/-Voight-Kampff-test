// VTK interface which generates the image

#include "VisualizeEvent.h"

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkOutlineFilter.h>
#include <vtkRenderWindowInteractor.h>
#include<vtkCubeSource.h>

// Code based on tutorial step 1

int DisplayData(const std::vector<EventRecord>& data)
{
	
	
	
	// Calling the min/max here to see the data output - later would display 
	// Also should put these and the computed scaling factor into some kind of class
	// And have individual functions for each of these, and make the object or struct const
	
    double min_northing{0};
    double max_northing{0};
    double min_easting{0};
    double max_easting{0};
    double min_depth{0};
	double max_depth{0};
	
	auto bbResult = findBoundingBox(data, min_northing, max_northing, 
									min_easting, max_easting, min_depth, max_depth); 

	
	// Debug print statements - not to be left
	std::cout << "Min Northing = " << min_northing << std::endl;
	std::cout << "Max Northing = " << max_northing << std::endl;
	std::cout << "Min Easting = " << min_easting << std::endl;
	std::cout << "Max Easting = " << max_easting << std::endl;
	std::cout << "Min Depth = " << min_depth << std::endl;
	std::cout << "Max Depth = " << max_depth << std::endl;
	
	
  vtkNew<vtkNamedColors> colors;
  
  
  //
  // Create the Renderer 
  //
  vtkNew<vtkRenderer> ren1;
  ren1->SetBackground(colors->GetColor3d("Black").GetData());

  
  

	double scalingFactor {26}; // Need to compute this based on the values seen
 
    for (EventRecord item : data) {
		vtkSmartPointer<vtkSphereSource> displaySphere = vtkNew<vtkSphereSource>();
		vtkSmartPointer<vtkPolyDataMapper> displayMapper = vtkNew<vtkPolyDataMapper>();
		vtkSmartPointer<vtkActor> displayActor = vtkNew<vtkActor>() ;
        
		
		displaySphere->SetCenter(item.Easting,item.Northing,item.Depth);
		displaySphere->SetRadius(item.MomentMagnitude * scalingFactor);

        displayMapper->SetInputConnection(displaySphere->GetOutputPort());

		
		displayActor->SetMapper(displayMapper);
		displayActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());  
		
		ren1->AddActor(displayActor);


    }
	
	// Add the bounding box (not using the outline filter because I haven't quite figured 
	// it out - would be the next step
	
	auto boundingBox = vtkNew<vtkCubeSource>();
	boundingBox->SetXLength(max_easting - min_easting);
	boundingBox->SetYLength(max_northing - min_northing);
	boundingBox->SetZLength(max_depth - min_depth);
	
	boundingBox->SetCenter((max_easting - min_easting)/2 + min_easting,
	                       (max_northing - min_northing)/2 + min_northing,
						   (max_depth - min_depth)/2 + min_depth);
 
	auto boundingMapper = vtkNew<vtkPolyDataMapper>();
	auto boundingActor = vtkNew<vtkActor>() ;
 
    boundingMapper->SetInputConnection(boundingBox->GetOutputPort());
	boundingActor->SetMapper(boundingMapper);
	boundingActor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());  
	
	boundingActor->GetProperty()->SetRepresentationToWireframe();  
	boundingActor->GetProperty()->SetLineWidth(2.0);
		
	ren1->AddActor(boundingActor);


  // Finally we create the render window which will show up on the screen.
  // We put our renderer into the render window using AddRenderer. 
  //
  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren1);
  renWin->SetSize(1000, 1000);
  renWin->SetWindowName("DisplayData");



  for (int i = 0; i < 1440; ++i)
  {
    // Render the image
    renWin->Render();
    // Rotate the active camera by one degree.
    ren1->GetActiveCamera()->Azimuth(1);
  }
  
  return 0;
	
}

int findBoundingBox(const std::vector<EventRecord>& data, 
                    double &min_northing,
                    double &max_northing,
                    double &min_easting,
                    double &max_easting,
					double &min_depth,
					double &max_depth)
{
	// Don't have a much error handling yet
	
    if (data.empty()) return -1; 

    auto compareNorthing = [](const EventRecord& a, const EventRecord& b){ return a.Northing < b.Northing; };
    auto compareEasting = [](const EventRecord& a, const EventRecord& b){ return a.Easting < b.Easting; };
    auto compareDepth = [](const EventRecord& a, const EventRecord& b){ return a.Depth < b.Depth; };
    auto itMinNorthing = std::min_element(data.begin(), data.end(), compareNorthing);
    auto itMaxNorthing = std::max_element(data.begin(), data.end(), compareNorthing);
    auto itMinEasting = std::min_element(data.begin(), data.end(), compareEasting);
    auto itMaxEasting = std::max_element(data.begin(), data.end(), compareEasting);
    auto itMinDepth = std::min_element(data.begin(), data.end(), compareDepth);
    auto itMaxDepth = std::max_element(data.begin(), data.end(), compareDepth);
	
	min_northing = itMinNorthing->Northing;
	max_northing = itMaxNorthing->Northing;
	min_easting = itMinEasting->Easting;
	max_easting = itMaxEasting->Easting;
	min_depth = itMinDepth->Depth;
	max_depth = itMaxDepth->Depth;
	
	return 0;
}
					