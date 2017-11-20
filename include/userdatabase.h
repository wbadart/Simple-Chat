/**
 * include/userdatabase.h
 *
 * Interface to simple user management database.
 *
 * Rob Simari
 * Royce Branning
 * Will Badart
 *
 * created: NOV 2017
 **/

#pragma once

#include <fstream>  // std::fstream
#include <map>      // std::map
#include <set>      // std::set
#include <string>   // std::string, std::getline


class UserDatabase {
  public:
    // Default constructor with path to data file
    UserDatabase(const std::string& path);

    // Default destructor (mainly just closes db file stream)
    ~UserDatabase();

    // Write a new user to the database and report success
    bool add_user(
        const std::string& username, const std::string& password);

    // Lookup a password by username
    std::string query(const std::string& username) const;

    // Authenticate user and record online status
    bool login(
        const std::string& username, const std::string& password);

    // Take a user offline
    void logout(const std::string& username);

  private:
    // Report whether the provided credentials match DB
    bool _authenticate(
        const std::string& username, const std::string& password) const;

    // Report whether a username is valid
    bool _valid_username(const std::string& username);

    // Report whether a username is valid
    bool _valid_password(const std::string& password);

    // Hold the path to the data file
    std::string m_path;

    // Filestream object of data file
    std::fstream* m_datafs;

    // Cache query results (map usernames to User)
    std::map<std::string, std::string> m_cache;

    // Store all currently authenticated users
    std::set<std::string> m_online;
};
