
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include <UnitTest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(intersection_queries_public_toronto_canada) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                loadMap("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test setup: loadMap threw an exceptinon !!!!" << std::endl;
                throw; // re-throw exceptinon
            }
        }
    
        ~BaseMapFixture() {
            //Clean-up
            try {
                closeMap();
            } catch (const std::exception& e) {
                std::cout << "!!!! BaseMapFixture test teardown: closeMap threw an exceptinon. what(): " << e.what() << " !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test teardown: closeMap threw an exceptinon !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            }
        }
    };


    struct MapFixture : BaseMapFixture {};

    TEST_FIXTURE(MapFixture, intersection_street_segments) {
        std::vector<StreetSegmentIdx> expected;

        expected = {5, 7811, 102107};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(10)));

        expected = {24821, 24822, 36814};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(27101)));

        expected = {27049, 27050, 27056};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(29435)));

        expected = {32703, 75704, 75705};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(34975)));

        expected = {36718, 36719, 36721};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(38701)));

        expected = {39235, 39236, 39323};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(40756)));

        expected = {40679, 124834, 212566, 212567};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(42199)));

        expected = {68750, 68767, 128254};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(65038)));

        expected = {82678, 136261, 136262};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(74966)));

        expected = {91207, 91431};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(81186)));

        expected = {93174, 180669, 180673, 180682};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(82672)));

        expected = {98040, 98041, 106569};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(86334)));

        expected = {99080, 99154, 99158};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(87076)));

        expected = {111298, 111318, 111322, 141351, 159502};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(95563)));

        expected = {121876};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(102380)));

        expected = {123437, 123438, 134241, 134243};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(103341)));

        expected = {127437, 127460, 127461};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(105969)));

        expected = {128935, 128936, 128949};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(106896)));

        expected = {132911, 132912, 132922};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(109313)));

        expected = {139808, 139818, 139819};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(113281)));

        expected = {147526, 181228, 181237};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(117691)));

        expected = {151332, 218770};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(119658)));

        expected = {155838, 155843};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(121985)));

        expected = {178999, 179000, 179023};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(133821)));

        expected = {179810, 218811};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(134204)));

        expected = {182658, 182659, 195593};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(135221)));

        expected = {186606, 186609, 186610};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(136515)));

        expected = {187909, 187910, 187918, 187919};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(137127)));

        expected = {202703, 202704, 211356};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(145491)));

        expected = {213128, 213131, 213132};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(149474)));

    } //intersection_street_segments

    TEST_FIXTURE(MapFixture, intersection_street_names) {
        std::vector<std::string> expected;

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(102380)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(121985)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(38701)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(40756)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(87076)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(105969)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(106896)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(109313)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(113281)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(133821)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(136515)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(145491)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(149474)));

        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(137127)));

        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(95563)));

        expected = {"<unknown>", "Bovaird Drive East", "Bovaird Drive East"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(117691)));

        expected = {"<unknown>", "Denison Street", "Denison Street"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(135221)));

        expected = {"<unknown>", "Highway 401 Express", "Highway 401 Express"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(10)));

        expected = {"<unknown>", "Kitimat Road", "Kitimat Road"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(27101)));

        expected = {"<unknown>", "North Service Road", "North Service Road"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(86334)));

        expected = {"<unknown>", "South Millway", "South Millway"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(34975)));

        expected = {"Athabasca Common", "Wheat Boom Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(134204)));

        expected = {"Claremont Street", "Queen Street West", "Queen Street West"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(74966)));

        expected = {"Constitution Boulevard", "Constitution Boulevard", "Vera Cruz Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(29435)));

        expected = {"Don Mills Road", "Don Mills Road", "Eglinton Avenue East", "Eglinton Avenue East"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(42199)));

        expected = {"Don Mills Road", "Don Mills Road", "York Mills Road", "York Mills Road"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(103341)));

        expected = {"Hurontario Street", "Hurontario Street", "Queensway", "Queensway East"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(82672)));

        expected = {"Isabella Street", "South Unionville Avenue", "South Unionville Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(65038)));

        expected = {"Moraine Drive", "Moraine Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(81186)));

        expected = {"The East Mall", "The East Mall"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(119658)));

    } //intersection_street_names

    TEST_FIXTURE(MapFixture, directly_connected) {
        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(10229, 96006)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(28262, 56035)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(32615, 87321)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(33063, 142972)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(38216, 82535)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(48021, 35797)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(72388, 48326)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(101772, 51749)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(117913, 42614)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(121164, 81132)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(125686, 45024)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(128969, 111876)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(140389, 82634)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(10, 10)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(27101, 38792)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(38701, 38701)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(40756, 40808)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(74966, 1109)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(82672, 24063)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(95563, 114114)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(105969, 105992)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(106896, 106897)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(109313, 109319)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(113281, 113278)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(117691, 134829)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(119658, 35816)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(121985, 121984)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(122694, 122695)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(135221, 102630)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(137127, 137128)));

    } //directly_connected

} //intersection_queries_public_toronto_canada

