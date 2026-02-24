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

        void addModule(std::string code, std::string name, int year_id, int credits, float grade);

        void addYear(int course_id, int weight, float grade);

        void addCourse(std::string name, float grade);

        // Edit objects (not grade)

        void editAssessment(int assessment_id, int n_weight);

        void editModule(int module_id, std::string n_code, std::string n_name, int n_credits);

        void editYear(int year_id, int n_weight);

        void editCourse(int course_id, std::string n_name);

        // Edit assessment grade + sync

        void updateAssessmentGrade(int assessment_id, float newGrade);

        void syncModule(int module_id);

        void syncYear(int year_id);

        void syncCourse(int course_id);

        // Delete objects

        void deleteAssessment(int assessment_id);

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

        // check whether the weight is valid, and whether weights in category add up to 100%
        // category_id is a module_id or course_id
        bool validateWeight(int category_id, int weight);

        bool validateCredits(int year_id, int credits);
    };

}

#endif