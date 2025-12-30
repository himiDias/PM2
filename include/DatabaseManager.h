#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "Types.h"

#include <string>
#include <sqlite3.h>
#include <stdexcept>

namespace prog_man
{

    class DatabaseManager
    {
    public:
        // constructor for a database manager

        /*
        Initialises database, creates if not exists
        Creates Course, Year , Module and Assessment Tables
        */
        DatabaseManager(std::string username);
        // destructor for a database manager
        ~DatabaseManager();

        // ADD table rows
        void addCourse(std::string course_name, float grade);

        void addYear(int year_num, int course_id, int weight, float grade);

        void addModule(std::string module_code, std::string module_name, int year_id, int credits, float grade);

        void addAssessment(AssessmentType assessment_type, int module_id, int weight, float grade);

        // Update table rows

        void updateCourse(int course_id, std::string new_course_name, float new_grade);

        void updateYear(int year_id, int new_year_num, int new_weight, float new_grade);

        void updateModule(int module_id, std::string new_module_code, std::string new_module_name, int new_credits, float new_grade);

        void updateAssessment(int assessment_id, AssessmentType new_assessment_type, int new_weight, float new_grade);

        // Delete table rows

        void deleteCourse(int course_id);

        void deleteYear(int year_id);

        void deleteModule(int module_id);

        void deleteAssessment(int assessment_id);

        // Get row info
        CourseData getCourse(int module_id);
        YearData getYear(int module_id);
        ModuleData getModule(int module_id);
        AssessmentData getAssessment(int module_id);

    private:
        sqlite3 *db_;
        std::string db_path_;

        void initialiseDB();
    };

}

#endif