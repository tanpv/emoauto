#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>


//struct report
struct reports {
    std::string id;
    int duration;
    int engagementValue;
    int excitementValue;
    int interestValue;
    int stressValue;
    int relaxValue;
    int focusValue;
    int session_rating;
    std::string time;  // time include date and hours with format "yyyy-MM-ddTHH:mm:ss";
    std::string photo;
    std::string notes;
    std::string name;

    reports() :id("0"), duration{ 0 }, engagementValue{ 0 }, excitementValue{ 0 }, interestValue{ 0 },
        stressValue{ 0 }, relaxValue{ 0 }, focusValue{ 0 }, session_rating{ 0 }, time(""),
        photo(""), notes(""), name("")
    {}
};

//parse respone report and store to File
std::unique_ptr<std::vector<reports>> parseReport(std::string input);

//write Report to File
void writeReport(std::unique_ptr<std::vector<reports>> list_report, std::ostream &os);