/*!
 * \file Highscore.cc
 * \brief File containing the Highscore class source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

#include "Highscore.h"

#include <algorithm>
#include <cctype>
#include <fstream>


Highscore::Highscore(const std::string &filename)
    : filename_(filename)
{
    for (auto & [score, xx] : highscore_) score = 0; // clear scores

    std::string highscore_str;

    if (this->readFileToString(highscore_str) != 0)
        this->readStringToArray(highscore_str);
}

Highscore::~Highscore() { save(); }

void Highscore::save() const
{
    std::ofstream highscore_of(this->filename_);

    if (highscore_of) {
        for (const auto & [score, name] : this->highscore_)
            highscore_of << score << name << '\n';
        highscore_of.close();
    }

}
std::string Highscore::get(unsigned n) const
{
    if (n < this->size())
        return std::to_string(this->highscore_[n].first) + " (" + this->highscore_[n].second + ")";
    return "0";
}

bool Highscore::add(size_t new_score)
{
    bool new_highscore = false;
    std::pair<size_t, std::string> temp;

    for (size_t i = 0; i < this->size(); ++i) {
        if (new_highscore) {
            std::swap(temp, this->highscore_[i]);
        } else if (new_score >= this->highscore_[i].first) {
            temp = this->highscore_[i];
            this->highscore_[i] = {new_score, "YOU!"};
            new_highscore = true;
        }
    }

    if (new_highscore) save();

    return new_highscore;
}

void Highscore::setNickname(const std::string &nickname)
{
    bool found = false;
    for (size_t i = 0; i < this->size(); ++i)
        if (this->highscore_[i].second == "YOU!") {
            this->highscore_[i].second = nickname;
            found = true;
        }
    if (found) save();
}

size_t Highscore::size() const { return highscore_.size(); }

int Highscore::readFileToString(std::string &highscore_string) const
{
    highscore_string.clear();

    std::ifstream highscore_if(this->filename_);
    if (highscore_if) {
        /* Resize highscore_str to fit whole file */
        highscore_if.seekg(0, std::ios::end);
        highscore_string.resize(highscore_if.tellg());
        highscore_if.seekg(0, std::ios::beg);

        /* Read file to highscore_str */
        highscore_if.get(&highscore_string[0], highscore_string.size(), '\0');
        highscore_if.close();
    }

    return highscore_string.size();
}

void Highscore::readStringToArray(const std::string &highscore_string)
{
    /* Read string to highscore array */
    size_t it = 0;
    size_t strpos = 0;
    while (it < this->size() && strpos < highscore_string.size()) {
        if (std::isdigit(highscore_string[strpos])) {
            this->highscore_[it].first = std::stoul(highscore_string.substr(strpos));
            while (std::isdigit(highscore_string[strpos]))
                ++strpos;
        } else {
            size_t string_start = strpos;
            while (!std::isdigit(highscore_string[strpos]) && strpos < highscore_string.size())
                ++strpos;
            this->highscore_[it++].second = highscore_string.substr(string_start, (strpos - string_start - 1));
        }
    }

    /* Sort the highscore array descending */
    std::sort(highscore_.begin(), highscore_.end(), [](auto &a, auto &b) { return a.first > b.first; });
}
