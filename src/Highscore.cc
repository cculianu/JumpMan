#include <fstream>
#include <algorithm>
#include <iostream>

#include "Highscore.hh"

using namespace std;

Highscore::Highscore(const string &filename) : 
  filename_(filename),
  highscore_{{0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, 
             {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}}
{
  string highscore_str;

  /* Set highscore_ to values from highscore-file */
  if (this->readFileToString(highscore_str) != 0)
    this->readStringToArray(highscore_str);
}

Highscore::~Highscore()
{
  ofstream highscore_of(this->filename_);

  if (highscore_of)
  {
    for (pair<size_t, string> score : this->highscore_)
      highscore_of << score.first << score.second << '\n';
    highscore_of.close();
  }
}

string Highscore::get(unsigned n) const
{
  if (n < this->size())
    return 
      to_string(this->highscore_[n].first) + 
      " (" + this->highscore_[n].second + ")";
  return "0";
}

bool Highscore::add(size_t new_score)
{
  bool new_highscore = false;

  pair<size_t, string> temp;
  for (size_t i = 0; i < this->size(); ++i)
  {
    if (new_highscore)
      swap(temp, this->highscore_[i]);
    else if (new_score >= this->highscore_[i].first)
    {
      temp = this->highscore_[i];
      this->highscore_[i] = {new_score, "YOU!"};
      new_highscore = true;
    }
  }

  return new_highscore;
}

void Highscore::setNickname(const std::string &nickname)
{
  for (size_t i = 0; i < this->size(); ++i)
    if (this->highscore_[i].second == "YOU!")
      this->highscore_[i].second = nickname;
}

size_t Highscore::size() const
{
  return sizeof(this->highscore_) / sizeof(pair<size_t, string>);
}

int Highscore::readFileToString(string &highscore_string) const
{
  highscore_string.clear();

  ifstream highscore_if(this->filename_);
  if (highscore_if)
  {
    /* Resize highscore_str to fit whole file */
    highscore_if.seekg(0, ios::end);
    highscore_string.resize(highscore_if.tellg());
    highscore_if.seekg(0, ios::beg);

    /* Read file to highscore_str */
    highscore_if.get(&highscore_string[0], highscore_string.size(), '\0');
    highscore_if.close();
  }

  return highscore_string.size();
}

void Highscore::readStringToArray(const string &highscore_string)
{
  /* Read string to highscore array */
  size_t it = 0;
  size_t strpos = 0;
  while (it < this->size() && strpos < highscore_string.size())
  {
    if (isdigit(highscore_string[strpos]))
    {
      this->highscore_[it].first = stoul(highscore_string.substr(strpos));
      while (isdigit(highscore_string[strpos]))
        ++strpos;
    }
    else
    {
      size_t string_start = strpos;
      while (!isdigit(highscore_string[strpos]) && 
             strpos < highscore_string.size())
        ++strpos;
      this->highscore_[it++].second = 
        highscore_string.substr(string_start, (strpos - string_start -1));
    }
  }

  /* Sort the highscore array descending */
  sort(this->highscore_, this->highscore_ + this->size(),
       [](pair<size_t, string> a, pair<size_t, string> b)
       { return a.first > b.first; });
}
