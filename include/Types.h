#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

namespace prog_man
{

    // Defined here so it's accessible to both the DB and UI
    enum class AssessmentType
    {
        COURSEWORK,
        EXAM
    };

    struct AssessmentData
    {
        int id;
        int module_id;
        AssessmentType type;
        int weight;
        int grade;
    };

    struct ModuleData
    {
        int id;
        int year_id;
        std::string module_code;
        std::string module_name;
        int credits;
        int grade;
        std::vector<AssessmentData> assessments;
    };

    struct YearData
    {
        int id;
        int course_id;
        int year_num;
        int weight;
        int grade;
        std::vector<ModuleData> modules;
    };

    struct CourseData
    {
        int id;
        std::string course_name;
        int grade;
        std::vector<YearData> years;
    };

}

#endif