#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <set>
#include <QString>

std::string set_to_string(std::set<int> set_to_convert);
std::set<int> string_to_set(std::string string_to_convert);
bool file_exists(const std::string & path);
bool removeRecursively(const QString &Path);
bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);

#endif //UTILS_H
