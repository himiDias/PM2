#ifndef DATA_CONTROLLER_H
#define DATA_CONTROLLER_H

#include "DatabaseManager.h"

namespace prog_man
{
    class DataController
    {
    public:
        DataController(DatabaseManager manager);

        // Adding objects

        void addAssessment(std::string type, int module_id, int weight, float grade);

        void addModule(std::string code, std::string name, int year_id, int credits);

        void addYear(int course_id, int weight, int year_num);

        void addCourse(std::string name);

        // Edit objects (not grade)

        void editAssessment(int assessment_id, int module_id, int n_weight, float n_grade);

        void editModule(int module_id, int year_id, std::string n_code, std::string n_name, int n_credits, float grade);

        void editYear(int year_id, int course_id, int n_weight, float grade);

        void editCourse(int course_id, std::string n_name, float grade);

        // Delete objects

        void deleteAssessment(int assessment_id, int module_int);

        void deleteModule(int module_id);

        void deleteYear(int year_id);

        void deleteCourse(int course_id);

        // Object Info

        ModuleData getModuleInfo(int module_id);

        YearData getYearInfo(int year_id);

        CourseData getCourseInfo(int course_id);

        // Extra actions

        // ADD STUFF HERE FOR FEATURES like, How much needed for required grade etc
        // Prediction of final Grade

    private:
        DatabaseManager manager;

        // sync after new assessment grade

        void syncModule(int module_id);

        void syncYear(int year_id);

        void syncCourse(int course_id);

        // check whether the weight is valid, and whether weights in category add up to 100%
        bool validateModuleWeights(int module_id, int *assessment_id, int weight);

        bool validateCourseWeights(int course_id, int *year_id, int weight);

        // check wehterh credits are valid andad dup to 120
        bool validateCredits(int year_id, int *module_id, int credits);
    };

}

#endif