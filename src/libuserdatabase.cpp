/**
 * src/libuserdatabase.cpp
 *
 * Implementation of simple user database.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: NOV 2017
 **/

#include <iostream>
#include <userdatabase.h>


UserDatabase::UserDatabase(const std::string& path)
        : m_path(path), m_datafs(std::fstream(path)) {
    for(std::string user; std::getline(m_datafs, user);) {
        // Skip comments (e.g. "DO NOT EDIT" remark) and blanks in data file
        if(user.size() == 0 || user.at(0) == '#') continue;

        const std::string::size_type delim_pos = user.find("|");
        const std::string
            username = user.substr(0, delim_pos),
            password = user.substr(delim_pos + 1, std::string::npos);

        m_cache.emplace(username, User{username, password});
    }

    // Reset error flag
    m_datafs.clear();
}


UserDatabase::~UserDatabase() { m_datafs.close(); }


bool UserDatabase::add_user(
        const std::string& username,
        const std::string& password) {
    if(m_cache.count(username)) return false;
    m_cache.emplace(username, User{username, password});
    m_datafs << std::endl << username << "|" << password;
    return m_datafs.good();
}


User UserDatabase::query(const std::string& username) const {
    return m_cache.at(username);
}
