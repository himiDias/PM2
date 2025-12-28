#include "DatabaseManager.h"

#include <iostream>

namespace prog_man
{

    DatabaseManager::DatabaseManager(std::string username)
    {
        db_path_ = "data/" + username + ".db";

        int ret_code = sqlite3_open(db_path_.c_str(), &db_);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("DatabaseManager.cpp || LOGIN FAILURE : Could not create/open user file.");
        }

        initialiseDB();
        
    }

    void DatabaseManager::initialiseDB()
    {
        std::string stmt = "";

        char *errorMessage;

        int ret_code = sqlite3_exec(db_, stmt.c_str(), nullptr, nullptr, &errorMessage);

        if (ret_code != SQLITE_OK)
        {
            std::string err = errorMessage;
            sqlite3_free(errorMessage);
            throw std::runtime_error("DatabaseManager.cpp || DATABASE INITIALISATION FAILURE : Could not create tables : " + err);
        }
    }

    DatabaseManager::~DatabaseManager()
    {
    }

    void DatabaseManager::addCourse(std::string course_name, int grade)
    {
    }

    void DatabaseManager::addYear(int year_num, int course_id, int weight, int grade)
    {
    }

    void DatabaseManager::addModule(std::string module_code, std::string module_name, int year_id, int credits, int grade)
    {
    }

    void DatabaseManager::addAssessment(AssessmentType assessment_type, int module_id, int weight, int grade)
    {
    }

    void DatabaseManager::updateCourse(int course_id, std::string new_course_name, int new_grade)
    {
    }

    void DatabaseManager::updateYear(int year_id, int new_year_num, int new_weight, int new_grade)
    {
    }

    void DatabaseManager::updateModule(int module_id, std::string new_module_code, std::string new_module_name, int new_credits, int new_grade)
    {
    }

    void DatabaseManager::updateAssessment(int assessment_id, AssessmentType new_assessment_type, int new_weight, int new_grade)
    {
    }

    void DatabaseManager::deleteCourse(int course_id)
    {
    }

    void DatabaseManager::deleteYear(int year_id)
    {
    }

    void DatabaseManager::deleteModule(int module_id)
    {
    }

    void DatabaseManager::deleteAssessment(int assessment_id)
    {
    }

    CourseData DatabaseManager::getCourse(int course_id)
    {
        return {};
    }

    YearData DatabaseManager::getYear(int year_id)
    {
        return {};
    }

    ModuleData DatabaseManager::getModule(int module_id)
    {
        return {};
    }

    AssessmentData DatabaseManager::getAssessment(int assessment_id)
    {
        return {};
    }
}