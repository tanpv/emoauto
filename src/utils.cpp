#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <fstream>
#include "utils.h"

using boost::property_tree::ptree;

//parse respone report and store to File
std::unique_ptr<std::vector<reports>> parseReport(std::string input)
{
    std::cout << "Parse Report to File " << std::endl;
    std::unique_ptr<std::vector<reports>> list_report(new std::vector<reports>);
    //std::vector<reports> list_report;
    try {
        ptree pt2;
        std::string report_string = input;
        std::istringstream is(report_string);
        read_json(is, pt2);
        /*report_string = pt2.get<std::string>("count");
        if (report_string.length() == 0)
            return;
        int number_report = atoi(report_string.c_str());*/
        BOOST_FOREACH(boost::property_tree::ptree::value_type & rowPair,
            pt2.get_child("results")) {
            reports current_report;
            BOOST_FOREACH(boost::property_tree::ptree::value_type & itemPair,
                rowPair.second) {

                if (itemPair.first == "collection_end") {
                    current_report.time = itemPair.second.get<std::string>("");
                }
                if (itemPair.first == "experiment_name") {
                    current_report.name = itemPair.second.get<std::string>("");
                }

                if (itemPair.first == "session_guid") {
                    current_report.id = itemPair.second.get<std::string>("");
                }
                if (itemPair.first == "note") {
                    std::string note = itemPair.second.get<std::string>("");
                    if (note.compare("null") != 0)
                        current_report.notes = note;
                    else
                        current_report.notes = "";
                }
                if (itemPair.first == "report_json") {
                    if (itemPair.second.get<std::string>("").compare("null") != 0) {
                        std::string report_string = itemPair.second.get<std::string>("");
                        std::istringstream is(report_string);
                        ptree pt3;
                        read_json(is, pt3);
                        if (pt3.get<std::string>("status").compare("false") != 0) {
                            current_report.session_rating = atoi(pt3.get<std::string>("sessionRating").c_str());
                            BOOST_FOREACH(boost::property_tree::ptree::value_type & rowPair,
                                pt3.get_child("activityScores")) {
                                BOOST_FOREACH(boost::property_tree::ptree::value_type & scorePair,
                                    rowPair.second) {
                                    if (scorePair.first == "engagement") {
                                        current_report.engagementValue = atoi(scorePair.second.get_value<std::string>(" ").c_str());
                                    }
                                    if (scorePair.first == "excitement") {
                                        current_report.excitementValue = atoi(scorePair.second.get_value<std::string>(" ").c_str());
                                    }
                                    if (scorePair.first == "focus") {
                                        current_report.focusValue = atoi(scorePair.second.get_value<std::string>(" ").c_str());
                                    }
                                    if (scorePair.first == "meditation") {
                                        current_report.relaxValue = atoi(scorePair.second.get_value<std::string>(" ").c_str());
                                    }
                                    if (scorePair.first == "valence") {
                                        current_report.interestValue = atoi(scorePair.second.get_value<std::string>(" ").c_str());
                                    }
                                    if (scorePair.first == "frustration") {
                                        current_report.stressValue = atoi(scorePair.second.get_value<std::string>(" ").c_str());
                                    }
                                    if (scorePair.first == "duration") {
                                        current_report.duration = atoi(scorePair.second.get_value<std::string>(" ").c_str());
                                    }
                                }
                            }
                        }
                    }
                }
            }
            list_report->push_back(current_report);
        }
        return list_report;

    }
    catch (std::exception & e) {
        std::cout << "Exception " << e.what();
        return nullptr;
    }

}

//write Report to File
void writeReport(std::unique_ptr<std::vector<reports>> list_report, std::ostream &os)
{
    std::cout << "Write Report \n";
    //print header
    if (list_report != nullptr)
    {
        os << std::setw(10) << "Activity Type" << std::setw(20) << "Rating"
            << std::setw(20) << "Time" << std::setw(30) << "Engagement" << std::setw(10) << "Excitment"
            << std::setw(10) << "Interest" << std::setw(10) << "Relaxion" << std::setw(10) << "Stress"
            << std::setw(10) << "Focus" << '\n';
        //print conent  
        for (std::vector<reports>::const_iterator it = list_report->begin(); it != list_report->end(); ++it)
        {
            os << std::setw(10) << it->name << std::setw(20) << it->session_rating
                << std::setw(30) << it->time << std::setw(20) << it->engagementValue << std::setw(10) << it->excitementValue
                << std::setw(10) << it->interestValue << std::setw(10) << it->relaxValue << std::setw(10) << it->stressValue
                << std::setw(10) << it->focusValue << std::endl;
        }
    }
    
}