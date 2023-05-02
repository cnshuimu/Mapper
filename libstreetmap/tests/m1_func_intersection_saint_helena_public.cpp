
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

SUITE(intersection_queries_public_saint_helena) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                loadMap("/cad2/ece297s/public/maps/saint-helena.streets.bin");
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

        expected = {0, 402, 403};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(0)));

        expected = {47, 48};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(75)));

        expected = {51, 255};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(81)));

        expected = {62, 63};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(96)));

        expected = {90, 91};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(112)));

        expected = {122, 123};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(180)));

        expected = {142, 176, 457};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(207)));

        expected = {155, 325, 326};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(224)));

        expected = {159, 343, 344};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(228)));

        expected = {170, 327, 328};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(239)));

        expected = {171, 172, 179};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(241)));

        expected = {192, 463};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(264)));

        expected = {209, 210, 211};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(283)));

        expected = {212, 359, 360};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(286)));

        expected = {215, 220, 221};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(293)));

        expected = {216, 223, 224};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(295)));

        expected = {222, 223};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(302)));

        expected = {234, 235, 239};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(313)));

        expected = {250, 251};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(331)));

        expected = {270, 271};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(337)));

        expected = {322, 323, 385};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(370)));

        expected = {326, 327, 422, 459};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(371)));

        expected = {335, 336, 338};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(379)));

        expected = {337};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(374)));

        expected = {339};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(377)));

        expected = {364, 453};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(325)));

        expected = {384};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(402)));

        expected = {397, 398};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(107)));

        expected = {398, 399};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(116)));

        expected = {439, 440};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetSegmentsOfIntersection(413)));

    } //intersection_street_segments

    TEST_FIXTURE(MapFixture, intersection_street_names) {
        std::vector<std::string> expected;

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(374)));

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(377)));

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(402)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(75)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(81)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(107)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(116)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(264)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(302)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(325)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(331)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(337)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(413)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(207)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(228)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(241)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(293)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(295)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(379)));

        expected = {"<unknown>", "<unknown>", "The Pavement"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(283)));

        expected = {"<unknown>", "Casons", "Casons"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(286)));

        expected = {"<unknown>", "Commonwealth Avenue", "Commonwealth Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(224)));

        expected = {"<unknown>", "Commonwealth Avenue", "Commonwealth Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(239)));

        expected = {"<unknown>", "Commonwealth Avenue", "Commonwealth Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(370)));

        expected = {"<unknown>", "Commonwealth Avenue", "Commonwealth Avenue", "Unnamed Road"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(371)));

        expected = {"<unknown>", "Evergreen Drive", "Evergreen Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(313)));

        expected = {"<unknown>", "Longwood Avenue", "Longwood Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(0)));

        expected = {"Colt Sheds", "Colt Sheds"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(112)));

        expected = {"Cow Path", "Cow Path"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(180)));

        expected = {"Deadwood", "Deadwood"};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetNamesOfIntersection(96)));

    } //intersection_street_names

    TEST_FIXTURE(MapFixture, directly_connected) {
        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(28, 265)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(78, 155)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(90, 241)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(91, 395)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(105, 228)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(132, 99)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(200, 133)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(281, 143)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(326, 117)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(335, 224)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(347, 124)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(357, 309)));

        ECE297_CHECK(!intersectionsAreDirectlyConnected(std::make_pair(388, 228)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(0, 0)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(75, 76)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(107, 107)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(112, 137)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(207, 206)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(228, 219)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(264, 418)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(293, 298)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(295, 302)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(302, 298)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(313, 312)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(325, 393)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(331, 329)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(337, 76)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(339, 22)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(374, 376)));

        ECE297_CHECK(intersectionsAreDirectlyConnected(std::make_pair(379, 376)));

    } //directly_connected

} //intersection_queries_public_saint_helena

