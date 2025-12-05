// Input loader application - manages data input and validation


#pragma once

#include <string>
#include <vector>
#include <filesystem>

struct EventRecord
{
  double Northing = 0.0;
  double Easting = 0.0;
  double Depth = 0.0;
  double MomentMagnitude = 0.0;
};


/// Loads CSV file at |path| and extracts rows containing columns:
/// "Northing", "Easting", "Depth", and "Moment Magnitude" (case-insensitive,
/// spaces/underscores tolerated).
/// On success returns true and fills |out|. On failure returns false and
/// optional |error| receives a diagnostic message.
bool LoadEventRecords(const std::string& path, std::vector<EventRecord>& out, std::string* error = nullptr);
