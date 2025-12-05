// Define classes to handle input and storage of raw event data

#include "inputLoader.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{

static inline std::string ToLowerTrim(std::string s)
{
  // trim
  auto notspace = [](int ch) { return !std::isspace(ch); };
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), notspace));
  s.erase(std::find_if(s.rbegin(), s.rend(), notspace).base(), s.end());
  // tolower
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
  // normalize separators
  for (auto& c : s)
  {
    if (c == '_' || c == ' ')
      c = ' ';
  }
  return s;
}

// Parse one CSV line into fields. Handles quoted fields and doubled quotes per RFC4180.
static std::vector<std::string> ParseCsvLine(const std::string& line)
{
  std::vector<std::string> fields;
  std::string cur;
  bool inQuotes = false;

  for (size_t i = 0; i < line.size(); ++i)
  {
    char ch = line[i];
    if (inQuotes)
    {
      if (ch == '\"')
      {
        // Double quote inside quoted field -> escaped quote if followed by quote
        if (i + 1 < line.size() && line[i + 1] == '\"')
        {
          cur.push_back('\"');
          ++i;
        }
        else
        {
          inQuotes = false;
        }
      }
      else
      {
        cur.push_back(ch);
      }
    }
    else
    {
      if (ch == '\"')
      {
        inQuotes = true;
      }
      else if (ch == ',')
      {
        fields.push_back(cur);
        cur.clear();
      }
      else
      {
        cur.push_back(ch);
      }
    }
  }
  fields.push_back(cur);
  return fields;
}

// Try parse double, trimming whitespace first.
static bool TryParseDouble(const std::string& s, double& out)
{
  std::string t = s;
  // trim
  auto notspace = [](int ch) { return !std::isspace(ch); };
  t.erase(t.begin(), std::find_if(t.begin(), t.end(), notspace));
  t.erase(std::find_if(t.rbegin(), t.rend(), notspace).base(), t.end());
  if (t.empty())
    return false;
  try
  {
    size_t idx = 0;
    out = std::stod(t, &idx);
    return idx == t.size();
  }
  catch (...)
  {
    return false;
  }
}

} // namespace

bool LoadEventRecords(const std::string& path, std::vector<EventRecord>& out, std::string* error)
{
  out.clear();
  std::ifstream ifs(path);
  if (!ifs.is_open())
  {
    if (error)
      *error = "Failed to open file: " + path;
    return false;
  }

  std::string headerLine;
  if (!std::getline(ifs, headerLine))
  {
    if (error)
      *error = "Empty file or could not read header: " + path;
    return false;
  }

  auto headerFields = ParseCsvLine(headerLine);
  // map column name (normalized) -> index
  int idxNorthing = -1, idxEasting = -1, idxDepth = -1, idxMag = -1;

  for (size_t i = 0; i < headerFields.size(); ++i)
  {
    std::string key = ToLowerTrim(headerFields[i]);
    // normalize "moment magnitude" variants
    if (key == "northing" || key == "northing ")
      idxNorthing = static_cast<int>(i);
    else if (key == "easting" || key == "easting ")
      idxEasting = static_cast<int>(i);
    else if (key == "depth" || key == "depth ")
      idxDepth = static_cast<int>(i);
    else
    {
      // try common alternative names
      if (key.find("moment") != std::string::npos && key.find("magnitude") != std::string::npos)
      {
        idxMag = static_cast<int>(i);
      }
      else if (key == "mag" || key == "magnitude" || key == "moment_magnitude" ||
               key == "moment magnitude")
      {
        idxMag = static_cast<int>(i);
      }
      else if (key == "z" && idxDepth == -1)
      {
        idxDepth = static_cast<int>(i);
      }
    }
  }

  if (idxNorthing < 0 || idxEasting < 0 || idxDepth < 0 || idxMag < 0)
  {
    if (error)
    {
      std::ostringstream ss;
      ss << "Missing required columns. Found columns:";
      for (size_t i = 0; i < headerFields.size(); ++i)
        ss << " [" << headerFields[i] << "]";
      ss << ". Required: Northing, Easting, Depth, Moment Magnitude.";
      *error = ss.str();
    }
    return false;
  }

  std::string line;
  size_t lineNo = 1;
  while (std::getline(ifs, line))
  {
    ++lineNo;
    if (line.empty())
      continue;

    auto fields = ParseCsvLine(line);
    // allow lines with fewer fields (skip)
    if (fields.size() <= static_cast<size_t>(std::max(std::max(idxNorthing, idxEasting),
                                                       std::max(idxDepth, idxMag))))
    {
      // skip malformed line
      continue;
    }

    EventRecord rec;
    double v = 0.0;
    if (!TryParseDouble(fields[idxNorthing], v))
      continue;
    rec.Northing = v;

    if (!TryParseDouble(fields[idxEasting], v))
      continue;
    rec.Easting = v;

    if (!TryParseDouble(fields[idxDepth], v))
      continue;
    rec.Depth = v;

    if (!TryParseDouble(fields[idxMag], v))
      continue;
    rec.MomentMagnitude = v;

    out.push_back(rec);
  }

  return true;
}