#ifndef APP_VIEW_H
#define APP_VIEW_H

#include "DataController.h"

#include <stack>

namespace prog_man
{

    enum class View
    {
        Login,
        Courses,
        Years,
        Modules
    };

    struct NavPoint
    {
        View view;
        int id;
    };

    class AppView
    {
    public:
        AppView(DataController &ctrlr) : controller(ctrlr) {}

        void render();

    private:
        DataController &controller;
        std::stack<NavPoint> navStack;

        void showLogin();

        void showCourses();

        void showYears(int courseId);

        void showModules(int yearId);

        void showAssessments(int moduleId);
    };

}

#endif