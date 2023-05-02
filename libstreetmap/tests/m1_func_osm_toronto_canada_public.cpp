
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

SUITE(osm_queries_public_toronto_canada) {

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

    TEST_FIXTURE(MapFixture, osm_node) {
        LatLon expected;

        expected = LatLon(43.63235855102539062, -79.78104400634765625);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6195883803)));

        expected = LatLon(43.63481903076171875, -79.77280426025390625);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6195879847)));

        expected = LatLon(43.63721466064453125, -79.77260589599609375);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6195885314)));

        expected = LatLon(43.66053390502929688, -79.76387023925781250);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6202576791)));

        expected = LatLon(43.66179656982421875, -79.74708557128906250);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6192979664)));

        expected = LatLon(43.66490936279296875, -79.55559539794921875);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6162757066)));

        expected = LatLon(43.66637039184570312, -79.46231842041015625);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6208907826)));

        expected = LatLon(43.67992019653320312, -79.45789337158203125);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6199043649)));

        expected = LatLon(43.68597412109375000, -79.44703674316406250);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6204536556)));

        expected = LatLon(43.69590759277343750, -79.44700622558593750);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(2567220992)));

        expected = LatLon(43.69793319702148438, -79.43311309814453125);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6196756961)));

        expected = LatLon(43.70679092407226562, -79.42621612548828125);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6201210569)));

        expected = LatLon(43.70997619628906250, -79.42578887939453125);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(803435352)));

        expected = LatLon(43.71014785766601562, -79.41799926757812500);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6195757864)));

        expected = LatLon(43.73124313354492188, -79.41290283203125000);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6195445268)));

        expected = LatLon(43.73209381103515625, -79.40059661865234375);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6197224679)));

        expected = LatLon(43.73451614379882812, -79.39896392822265625);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6197225053)));

        expected = LatLon(43.73522567749023438, -79.34115600585937500);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6208339872)));

        expected = LatLon(43.76689910888671875, -79.33689117431640625);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(18318863)));

        expected = LatLon(43.77881622314453125, -79.31923675537109375);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6199288852)));

        expected = LatLon(43.78248977661132812, -79.18341827392578125);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(3310289725)));

        expected = LatLon(43.79155349731445312, -79.18247222900390625);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6179929608)));

        expected = LatLon(43.79500579833984375, -79.11517333984375000);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(1839240573)));

        expected = LatLon(43.82990264892578125, -79.09699249267578125);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(2236220057)));

        expected = LatLon(43.83987808227539062, -79.08709716796875000);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(731833731)));

        expected = LatLon(43.84554672241210938, -79.07513427734375000);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(1382484595)));

        expected = LatLon(43.84934234619140625, -79.07128143310546875);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(1742181553)));

        expected = LatLon(43.86786270141601562, -79.06166076660156250);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(896464686)));

        expected = LatLon(43.87440490722656250, -79.01911926269531250);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(842901907)));

        expected = LatLon(43.88218307495117188, -79.00913238525390625);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(4036674075)));

    } //osm_node

} //osm_queries_public_toronto_canada

