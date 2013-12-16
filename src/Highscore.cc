#include <fstream>
#include <algorithm>
#include <iostream>

#include "Highscore.hh"

using namespace std;

Highscore::Highscore(const string &filename) : 
  filename_(filename),
  highscore_{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
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
    for (size_t score : this->highscore_)
      highscore_of << score << '\n';
    highscore_of.close();
  }
}

size_t Highscore::get(unsigned n)
{
  const size_t highscore_size = sizeof(this->highscore_) / sizeof(size_t);
  if (n < highscore_size)
    return this->highscore_[n];
  return 0;
}

bool Highscore::add(size_t new_score)
{
  bool new_highscore = false;

  size_t temp;
  for (int i = 0; i < 10; ++i)
  {
    if (new_highscore)
      swap(temp, this->highscore_[i]);
    else if (new_score >= this->highscore_[i])
    {
      temp = this->highscore_[i];
      this->highscore_[i] = new_score;
      new_highscore = true;
    }
  }

  return new_highscore;
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
  const size_t highscore_size = sizeof(this->highscore_) / sizeof(size_t);
  size_t highscore_it = 0;
  size_t strpos = 0;
  while (highscore_it < highscore_size && strpos < highscore_string.size())
  {
    if (isdigit(highscore_string[strpos]))
    {
      this->highscore_[highscore_it++] = stoul(highscore_string.substr(strpos));
      while (isdigit(highscore_string[strpos]))
        ++strpos;
    }
    else
      while (!isdigit(highscore_string[strpos]))
        ++strpos;
  }

  /* Sort the highscore array descending */
  sort(this->highscore_, this->highscore_+highscore_size,
       [](size_t a, size_t b){return a > b;});
}
