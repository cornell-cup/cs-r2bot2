#ifndef _R2BOT_GLOBAL
#define _R2BOT_GLOBAL

#include "Config.h"

#include <memory>
#include <unordered_map>
#include <vector>

using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

/** Pointer */
template<typename T> using ptr = shared_ptr<T>;
/** String => Class Pointer Map */
template<typename T> using smap = unordered_map<string, T>;

#endif