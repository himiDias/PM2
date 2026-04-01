#include "AppView.h"


#include <iostream>
#include "imgui/imgui.h"

namespace prog_man
{
    AppView::AppView(DataController &ctrlr) : controller(ctrlr)
    {
        navStack.push({View::Login, -1});
    }

    void AppView::render()
    {
        if (navStack.empty())
            return;

        NavPoint &current = navStack.top();

        switch (current.view)
        {
        case View::Login:
            showLogin();
            break;

        case View::Courses:
            showCourses();
            break;

        case View::Years:
            showYears(current.id);
            break;

        case View::Modules:
            showModules(current.id);
            break;

        case View::Assessments:
            showAssessments(current.id);
            break;
        }
    }

    void AppView::showLogin()
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f - 150,
                                   ImGui::GetIO().DisplaySize.y * 0.5f - 100));

        ImGui::BeginChild("LoginFrame", ImVec2(300, 150), true);

        ImGui::Text("Academic Progress Manager Login");
        ImGui::Separator();
        ImGui::Spacing();

        static char username[64] = "";

        ImGui::InputText("Username", username, IM_ARRAYSIZE(username));

        ImGui::Spacing();

        if (ImGui::Button("Enter System", ImVec2(-FLT_MIN, 0)))
        {
            if (strlen(username) > 0)
            {
                try
                {
                    controller.loginUser(username);

                    navStack.push({View::Courses, -1});
                }
                catch (std::exception &e)
                {
                    std::cout << "Login Error: " << e.what() << "\n";
                }
            }
        }

        ImGui::EndChild();
    }

    void AppView::showCourses()
    {
        ImGui::Text("Course View");
    }

    void AppView::showYears(int course_id)
    {
        ImGui::Text("Year View");
    }

    void AppView::showModules(int year_id)
    {
        ImGui::Text("Module View");
    }

    void AppView::showAssessments(int module_id)
    {
        ImGui::Text("Assessments View");
    }

}
