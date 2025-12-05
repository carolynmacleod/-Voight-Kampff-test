

#include "inputLoader.h" // This is a sign I need to introduce a separate data format header

int DisplayData(const std::vector<EventRecord>& data);
int findBoundingBox(const std::vector<EventRecord>& data, 
                    double &min_northing,
                    double &max_northing,
                    double &min_easting,
                    double &max_easting,
					double &min_depth,
					double &max_depth);