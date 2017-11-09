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

#include <fstream>  // std::fstream
#include <map>      // std::map
#include <string>   // std::string, std::getline


struct User {
    std::string username;
    std::string password;
};


class UserDatabase {
  public:
    // Default constructor with path to data file
    UserDatabase(const std::string& path="./users.db");

    // Default destructor (mainly just closes db file stream)
    ~UserDatabase();

    // Write a new user to the database and report success
    bool add_user(
        const std::string& username, const std::string& password);

    // Lookup a record by username
    User query(const std::string& username) const;

  private:
    // Hold the path to the data file
    std::string m_path;

    // Filestream object of data file
    std::fstream m_datafs;

    // Cache query results (map usernames to User)
    std::map<std::string, User> m_cache;
};
