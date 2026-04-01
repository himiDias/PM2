#include "DataController.h"

#include <string.h>
namespace prog_man
{
    DataController::DataController(DatabaseManager mgr) : manager(mgr)
    {
    }

    void DataController::loginUser(std::string username)
    {
        manager.connect(username);
    }

    void DataController::addAssessment(std::string type, int module_id, int weight, float grade)
    {
        if (grade < 0 || grade > 100)
        {
            throw std::invalid_argument("Invalid argument to grade : " + std::to_string(grade) + " , grade must be between 0 - 100");
        }

        if (!validateModuleWeights(module_id, nullptr, weight))
        {
            throw std::invalid_argument("Invalid argument to weight : " + std::to_string(weight) + " , weight must be between 0-100 and weights in module must add up to 100% ");
        }

        AssessmentType a_type;
        if (type == "COURSEWORK")
        {
            a_type = AssessmentType::COURSEWORK;
        }
        else
        {
            a_type = AssessmentType::EXAM;
        }

        manager.addAssessment(a_type, module_id, weight, grade);
        syncModule(module_id);
    }

    void DataController::addModule(std::string code, std::string name, int year_id, int credits)
    {
        if (!validateCredits(year_id, nullptr, credits))
        {
            throw std::invalid_argument("Invalid argument to credits : " + std::to_string(credits) + " , credits must be between 0-120 and credits in year must add up 120 ");
        }

        manager.addModule(code, name, year_id, credits, 0);
    }

    void DataController::addYear(int course_id, int weight, int year_num)
    {
        if (!validateCourseWeights(course_id, nullptr, weight))
        {
            throw std::invalid_argument("Invalid argument to weight : " + std::to_string(weight) + " , weight must be between 0-100 and weights in year must add up to 100% ");
        }

        manager.addYear(year_num, course_id, weight, 0);
    }

    void DataController::addCourse(std::string name)
    {
        manager.addCourse(name, 0);
    }

    void DataController::editAssessment(int assessment_id, int module_id, int n_weight, float n_grade)
    {
        if (n_grade < 0 || n_grade > 100)
        {
            throw std::invalid_argument("Invalid argument to grade : " + std::to_string(n_grade) + " , grade must be between 0 - 100");
        }

        if (!validateModuleWeights(module_id, &assessment_id, n_weight))
        {
            throw std::invalid_argument("Invalid argument to weight : " + std::to_string(n_weight) + " , weight must be between 0-100 and weights in module must add up to 100% ");
        }

        manager.updateAssessment(assessment_id, n_weight, n_grade);
        syncModule(module_id);
    }

    void DataController::editModule(int module_id, int year_id, std::string n_code, std::string n_name, int n_credits, float grade)
    {
        if (!validateCredits(year_id, &module_id, n_credits))
        {
            throw std::invalid_argument("Invalid argument to credits : " + std::to_string(n_credits) + " , credits must be between 0-120 and credits in year must add up 120 ");
        }

        manager.updateModule(module_id, n_code, n_name, n_credits, grade);
        syncModule(module_id);
    }

    void DataController::editYear(int year_id, int course_id, int n_weight, float grade)
    {
        if (!validateCourseWeights(course_id, &year_id, n_weight))
        {
            throw std::invalid_argument("Invalid argument to weight : " + std::to_string(n_weight) + " , weight must be between 0-100 and weights in course must add up to 100% ");
        }

        manager.updateYear(year_id, n_weight, grade);
        syncYear(year_id);
    }

    void DataController::editCourse(int course_id, std::string n_name, float grade)
    {
        manager.updateCourse(course_id, n_name, grade);
    }

    void DataController::deleteAssessment(int assessment_id, int module_id)
    {
        manager.deleteAssessment(assessment_id);
        syncModule(module_id);
    }

    void DataController::deleteModule(int module_id)
    {
        ModuleData data = getModuleInfo(module_id);
        manager.deleteModule(module_id);
        syncYear(data.year_id);
    }

    void DataController::deleteYear(int year_id)
    {
        YearData data = getYearInfo(year_id);
        manager.deleteYear(year_id);
        syncCourse(data.course_id);
    }

    void DataController::deleteCourse(int course_id) { manager.deleteCourse(course_id); }

    ModuleData DataController::getModuleInfo(int module_id) { return manager.getModule(module_id); }
    YearData DataController::getYearInfo(int year_id) { return manager.getYear(year_id); }
    CourseData DataController::getCourseInfo(int course_id) { return manager.getCourse(course_id); }
    std::vector<CourseSummary> DataController::getAllCoursesInfo() { return manager.getAllCourses(); }

    void DataController::syncModule(int module_id)
    {
        ModuleData data = getModuleInfo(module_id);

        float new_grade = 0.0f;

        for (AssessmentData ad : data.assessments)
        {
            new_grade += ad.grade * (ad.weight / 100.0f);
        }

        manager.updateModule(module_id, data.module_code, data.module_name, data.credits, new_grade);

        syncYear(data.year_id);
    }

    void DataController::syncYear(int year_id)
    {
        YearData data = getYearInfo(year_id);

        float new_grade = 0;

        for (ModuleSummary ms : data.modules)
        {
            new_grade += ms.grade * (ms.credits / 120.0f);
        }

        manager.updateYear(year_id, data.weight, new_grade);

        syncCourse(data.course_id);
    }

    void DataController::syncCourse(int course_id)
    {

        CourseData data = getCourseInfo(course_id);

        float new_grade = 0;

        for (YearSummary ys : data.years)
        {
            new_grade += ys.grade * (ys.weight / 100.0f);
        }

        manager.updateCourse(course_id, data.course_name, new_grade);
    }

    bool DataController::validateModuleWeights(int module_id, int *assessment_id, int weight)
    {
        if (weight < 0 || weight > 100)
        {
            return false;
        }

        ModuleData data = getModuleInfo(module_id);

        int sum_weights = 0;
        for (AssessmentData ad : data.assessments)
        {
            if (assessment_id != nullptr && ad.id == *assessment_id)
            {
                continue;
            }
            sum_weights += ad.weight;
        }

        if (sum_weights + weight > 100)
        {
            return false;
        }

        return true;
    }

    bool DataController::validateCourseWeights(int course_id, int *year_id, int weight)
    {
        if (weight < 0 || weight > 100)
        {
            return false;
        }

        CourseData data = getCourseInfo(course_id);

        int sum_weights = 0;
        for (YearSummary ys : data.years)
        {
            if (year_id != nullptr && ys.id == *year_id)
            {
                continue;
            }
            sum_weights += ys.weight;
        }

        if (sum_weights + weight > 100)
        {
            return false;
        }

        return true;
    }

    bool DataController::validateCredits(int year_id, int *module_id, int credits)
    {
        if (credits < 0 || credits > 120)
        {
            return false;
        }

        YearData data = getYearInfo(year_id);

        int sum_creds = 0;
        for (ModuleSummary ms : data.modules)
        {
            if (module_id != nullptr && ms.id == *module_id)
            {
                continue;
            }
            sum_creds += ms.credits;
        }

        if (sum_creds + credits > 120)
        {
            return false;
        }
        return true;
    }
}