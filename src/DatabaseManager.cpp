#include "DatabaseManager.h"

#include <iostream>
#include <sstream>

namespace prog_man
{

    DatabaseManager::DatabaseManager(std::string username)
    {
        db_path_ = "data/" + username + ".db";

        int ret_code = sqlite3_open(db_path_.c_str(), &db_);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("DatabaseManager.cpp || LOGIN FAILURE : Could not create/open user file. \n");
        }

        initialiseDB();
    }

    void DatabaseManager::initialiseDB()
    {
        std::string stmt = "PRAGMA foreign_keys=ON;";

        stmt += "CREATE TABLE IF NOT EXISTS courses(id INTEGER AUTOINCREMENT PRIMARY KEY,title TEXT,grade REAL)";
        stmt += "CREATE TABLE IF NOT EXISTS years(id INTEGER AUTOINCREMENT PRIMARY KEY,year INTEGER,weight INTEGER,grade REAL,course_id INTEGER,FOREIGN KEY(course_id) REFERENCES courses(id) ON DELETE CASCADE)";
        stmt += "CREATE TABLE IF NOT EXISTS modules(id INTEGER AUTOINCREMENT PRIMARY KEY,code TEXT,title TEXT,credits INTEGER,grade REAL,year_id INTEGER,FOREIGN KEY(year_id) REFERENCES years(id) ON DELETE CASCADE)";
        stmt += "CREATE TABLE IF NOT EXISTS assessments(id INTEGER AUTOINCREMENT PRIMARY KEY,type TEXT,weight INTEGER,grade REAL,module_id INTEGER,FOREIGN KEY(module_id) REFERENCES modules(id) ON DELETE CASCADE)";

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

    void DatabaseManager::addCourse(std::string course_name, float grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "INSERT INTO courses (title,grade) VALUES (?,?);", -1, &stmt, nullptr);
        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare add statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_text(stmt, 1, course_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, grade);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            throw std::runtime_error("DatabaseManager.cpp || DB INSERT FAILURE : Could not add course : " + course_name + "\n");
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::addYear(int year_num, int course_id, int weight, float grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "INSERT INTO years (year,weight,grade,course_id) VALUES (?,?,?,?);", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare add statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_double(stmt, 1, year_num);
        sqlite3_bind_int(stmt, 2, weight);
        sqlite3_bind_double(stmt, 3, grade);
        sqlite3_bind_int(stmt, 4, course_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            throw std::runtime_error("DatabaseManager.cpp || DB INSERT FAILURE : Could not add year : \n");
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::addModule(std::string module_code, std::string module_name, int year_id, int credits, float grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "INSERT INTO modules (code,title,credits,grade,year_id) VALUES (?,?,?,?,?);", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare add statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_text(stmt, 1, module_code.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, module_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, credits);
        sqlite3_bind_double(stmt, 4, grade);
        sqlite3_bind_int(stmt, 5, year_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            throw std::runtime_error("DatabaseManager.cpp || DB INSERT FAILURE : Could not add module : " + module_code + "\n");
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::addAssessment(AssessmentType assessment_type, int module_id, int weight, float grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "INSERT INTO assessments (type,weight,grade,module_id) VALUES (?,?,?,?);", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare add statement: " + std::string(sqlite3_errmsg(db_)));
        }

        std::string type;
        switch (assessment_type)
        {
        case AssessmentType::COURSEWORK:
            type = "COURSEWORK";
            break;
        case AssessmentType::EXAM:
            type = "EXAM";
            break;
        default:
            throw std::runtime_error("DatabaseManager.cpp || INVALID TYPE : Assessment type invalid \n");
        }

        sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, weight);
        sqlite3_bind_double(stmt, 3, grade);
        sqlite3_bind_int(stmt, 4, module_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            throw std::runtime_error("DatabaseManager.cpp || DB INSERT FAILURE : Could not add assessment \n");
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::updateCourse(int course_id, std::string new_course_name, float new_grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "UPDATE courses SET title = ?, grade = ? WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare update statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_text(stmt, 1, new_course_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, new_grade);
        sqlite3_bind_int(stmt, 3, course_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB UPDATE FAILURE : Could not update course : " << course_id << ",VALS:" << new_course_name << "," << new_grade << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::updateYear(int year_id, int new_weight, float new_grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "UPDATE years SET  weight = ?, grade = ? WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare update statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, new_weight);
        sqlite3_bind_double(stmt, 2, new_grade);
        sqlite3_bind_int(stmt, 3, year_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB UPDATE FAILURE : Could not update year : " << year_id << ",VALS:" << new_weight << "," << new_grade << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::updateModule(int module_id, std::string new_module_code, std::string new_module_name, int new_credits, float new_grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "UPDATE modules SET code = ?, title = ?, credits = ?, grade = ? WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare update statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_text(stmt, 1, new_module_code.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, new_module_name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, new_credits);
        sqlite3_bind_double(stmt, 4, new_grade);
        sqlite3_bind_int(stmt, 5, module_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB UPDATE FAILURE : Could not update module : " << module_id << ",VALS:" << new_module_code << "," << new_module_name << "," << new_credits << "," << new_grade << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::updateAssessment(int assessment_id, int new_weight, float new_grade)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "UPDATE assessments SET weight = ?, grade = ? WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare update statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, new_weight);
        sqlite3_bind_double(stmt, 2, new_grade);
        sqlite3_bind_int(stmt, 3, assessment_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB UPDATE FAILURE : Could not update assessment : " << assessment_id << ",VALS:" << new_weight << "," << new_grade << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::deleteCourse(int course_id)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "DELETE FROM courses WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare delete statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, course_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB DELETE FAILURE : Could not delete course : " << course_id << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::deleteYear(int year_id)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "DELETE FROM years WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare delete statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, year_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB DELETE FAILURE : Could not delete year : " << year_id << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::deleteModule(int module_id)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "DELETE FROM modules WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare delete statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, module_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB DELETE FAILURE : Could not delete module : " << module_id << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    void DatabaseManager::deleteAssessment(int assessment_id)
    {
        sqlite3_stmt *stmt;

        int ret_code = sqlite3_prepare_v2(db_, "DELETE FROM assessments WHERE id = ?", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare delete statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, assessment_id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::stringstream err;
            err << "DatabaseManager.cpp || DB DELETE FAILURE : Could not delete assessment : " << assessment_id << "\n";
            throw std::runtime_error(err.str());
        }

        sqlite3_finalize(stmt);
    }

    CourseData DatabaseManager::getCourse(int course_id)
    {
        sqlite3_stmt *stmt;

        CourseData data;

        int ret_code = sqlite3_prepare_v2(db_, "SELECT title, grade FROM courses WHERE id = ?;", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, course_id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            data.id = course_id;

            // Extracting columns (0-indexed based on your SELECT statement)
            const unsigned char *text = sqlite3_column_text(stmt, 0);

            data.course_name = reinterpret_cast<const char *>(text);
            data.grade = sqlite3_column_double(stmt, 1);
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Course ID " + std::to_string(course_id) + " not found.");
        }

        sqlite3_finalize(stmt);

        /*
            NEED TO GET YEARS ASSOCIATED TO COURSE
            //requires,
            Searching the years table, for course id that matches this course id
            for each year id, call getYearSummary, and put into an array
        */

        int ret_code_ys = sqlite3_prepare_v2(db_, "SELECT id FROM years WHERE course_id = ?;", -1, &stmt, nullptr);

        if (ret_code_ys != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_bind_int(stmt, 1, course_id);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int year_id = sqlite3_column_int(stmt, 0);

            YearSummary ys = getYearSummary(year_id);
            data.years.push_back(ys);
        }
        sqlite3_finalize(stmt);

        return data;
    }

    YearSummary DatabaseManager::getYearSummary(int year_id)
    {
        sqlite3_stmt *stmt;

        YearSummary summary;

        int ret_code = sqlite3_prepare_v2(db_, "SELECT year, weight, grade FROM years WHERE id = ?;", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, year_id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            summary.id = year_id;
            summary.year_num = sqlite3_column_int(stmt, 0);
            summary.weight = sqlite3_column_int(stmt, 1);
            summary.grade = sqlite3_column_double(stmt, 2);
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Year ID " + std::to_string(year_id) + " not found.");
        }

        sqlite3_finalize(stmt);

        return summary;
    }

    YearData DatabaseManager::getYear(int year_id)
    {
        sqlite3_stmt *stmt;

        YearData data;

        int ret_code = sqlite3_prepare_v2(db_, "SELECT year, weight, grade, course_id FROM years WHERE id = ?;", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, year_id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            data.id = year_id;
            data.year_num = sqlite3_column_int(stmt, 0);
            data.weight = sqlite3_column_int(stmt, 1);
            data.grade = sqlite3_column_double(stmt, 2);
            data.course_id = sqlite3_column_int(stmt, 3);
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Course ID " + std::to_string(year_id) + " not found.");
        }

        sqlite3_finalize(stmt);

        /*
            NEED TO GET MODULE ASSOCIATED TO YEAR
            //requires,
            Searching the module table, for module id that matches this year id
            for each module id, call getModuleSummary, and put into a vector
        */

        int ret_code_ms = sqlite3_prepare_v2(db_, "SELECT id FROM modules WHERE year_id = ?;", -1, &stmt, nullptr);

        if (ret_code_ms != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_bind_int(stmt, 1, year_id);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int module_id = sqlite3_column_int(stmt, 0);

            ModuleSummary ms = getModuleSummary(module_id);
            data.modules.push_back(ms);
        }
        sqlite3_finalize(stmt);

        return data;
    }

    ModuleSummary DatabaseManager::getModuleSummary(int module_id)
    {
        sqlite3_stmt *stmt;

        ModuleSummary summary;

        int ret_code = sqlite3_prepare_v2(db_, "SELECT code, title, credits, grade FROM modules WHERE id = ?;", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, module_id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            summary.id = module_id;

            const unsigned char *text = sqlite3_column_text(stmt, 0);
            summary.module_code = reinterpret_cast<const char *>(text);

            text = sqlite3_column_text(stmt, 1);
            summary.module_name = reinterpret_cast<const char *>(text);

            summary.credits = sqlite3_column_int(stmt, 2);
            summary.grade = sqlite3_column_double(stmt, 3);
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Module ID " + std::to_string(module_id) + " not found.");
        }

        sqlite3_finalize(stmt);

        return summary;
    }

    ModuleData DatabaseManager::getModule(int module_id)
    {
        sqlite3_stmt *stmt;

        ModuleData data;

        int ret_code = sqlite3_prepare_v2(db_, "SELECT code, title, credits, grade, year_id FROM modules WHERE id = ?;", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, module_id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            data.id = module_id;

            const unsigned char *text = sqlite3_column_text(stmt, 0);
            data.module_code = reinterpret_cast<const char *>(text);

            text = sqlite3_column_text(stmt, 1);
            data.module_name = reinterpret_cast<const char *>(text);

            data.credits = sqlite3_column_int(stmt, 2);
            data.grade = sqlite3_column_double(stmt, 3);

            data.year_id = sqlite3_column_int(stmt, 4);
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Module ID " + std::to_string(module_id) + " not found.");
        }

        sqlite3_finalize(stmt);

        /*
            NEED TO GET ASSESSMENTS ASSOCIATED TO MODULE
            //requires,
            Searching the assessments table, for assessment id that matches this module id
            for each assessment id, call getAssessment, and put into a vector
        */

        int ret_code_as = sqlite3_prepare_v2(db_, "SELECT id FROM assessments WHERE module_id = ?;", -1, &stmt, nullptr);

        if (ret_code_as != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_bind_int(stmt, 1, module_id);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int assessment_id = sqlite3_column_int(stmt, 0);

            AssessmentData ad = getAssessment(assessment_id);
            data.assessments.push_back(ad);
        }
        sqlite3_finalize(stmt);

        return data;
        return {};
    }

    AssessmentData DatabaseManager::getAssessment(int assessment_id)
    {
        sqlite3_stmt *stmt;

        AssessmentData data;

        int ret_code = sqlite3_prepare_v2(db_, "SELECT type, weight, grade, module_id FROM assessments WHERE id = ?;", -1, &stmt, nullptr);

        if (ret_code != SQLITE_OK)
        {
            throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(db_)));
        }

        sqlite3_bind_int(stmt, 1, assessment_id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            data.id = assessment_id;

            const unsigned char *text = sqlite3_column_text(stmt, 0);
            std::string typeStr = reinterpret_cast<const char *>(text);
            if (typeStr == "COURSEWORK")
            {
                data.type = AssessmentType::COURSEWORK;
            }
            else
            {
                data.type = AssessmentType::EXAM;
            }

            data.weight = sqlite3_column_int(stmt, 1);

            data.grade = sqlite3_column_double(stmt, 2);

            data.module_id = sqlite3_column_int(stmt, 3);
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Assessment ID " + std::to_string(assessment_id) + " not found.");
        }

        sqlite3_finalize(stmt);
        return data;
    }
}